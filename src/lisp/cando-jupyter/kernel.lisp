(in-package :cando-jupyter)

(defvar +display-name+ "Cando")
(defvar +language+ "cando")

(defparameter *leap-syntax* t)

(defun leap-syntax-enable (on)
  (setq *leap-syntax* on))

(defclass kernel (common-lisp-jupyter:kernel)
  ()
  (:default-initargs
    :package (find-package :cando-user)
    :banner "cando-jupyter: a Cando Jupyter kernel
(C) 2020 Christian Schafmeister (LGPL-3.0)"))

; The readtable was copied in the original code. Do we need to do that?
(defmethod jupyter:start :after ((k kernel))
  (let ((amber-home
          (namestring (or (if (ext:getenv "AMBERHOME")
                              (probe-file (ext:getenv "AMBERHOME"))
                              "/usr/local/amber/")
                          (probe-file "/home/app/amber16-data/")
                          "/dev/null"))))
    (setf (logical-pathname-translations "amber")
          (list (list "**;*.*" (concatenate 'string amber-home "/**/*.*"))))
    (jupyter:inform :info k "Setting amber host pathname translation -> ~a" amber-home)))


(defmethod jupyter:start :before ((k kernel))
  (when lparallel:*kernel*
    (error "The lparallel:*kernel* is not NIL and it must be when jupyter starts.   Add -f no-auto-lparallel to cando startup"))
  (setf lparallel:*kernel* (lparallel:make-kernel (core:num-logical-processors))))

(defmethod jupyter:stop :after ((k kernel))
  #+(or)(format t "jupyter:stop :after (mp:all-processes) -> ~s~%" (mp:all-processes))
  (lparallel:end-kernel :wait t))

(defun lisp-code-p (code)
  (do ((index 0 (1+ index)))
      ((>= index (length code)))
    (case (char code index)
      ((#\( #\*)
        (return t))
      ((#\space #\tab #\newline))
      (otherwise
        (return nil)))))

(defun leap-read (code)
  (architecture.builder-protocol:with-builder ('list)
    (esrap:parse 'leap.parser::leap code)))

(defun leap-eval (ast)
  (jupyter:handling-errors
    ; Not sure if leap needs its own result handler
    (jupyter:make-lisp-result
      (leap.core:evaluate 'list ast leap.core:*leap-env*))))

(defmethod jupyter:evaluate-code ((k kernel) code)
  (if (or (not *leap-syntax*)
          (lisp-code-p code))
    (call-next-method)
    (let ((ast (jupyter:handling-errors (leap-read code))))
      (if (typep ast 'jupyter:result)
        ast
        (list (leap-eval ast))))))


(defun leap-locate (ast cursor-pos &optional child-pos parents)
  (if (listp (car ast))
    (loop for child in ast
          for index from 0
          for fragment = (leap-locate child cursor-pos index parents)
          when fragment
          return fragment)
    (let ((bounds (getf (cddr ast) :bounds))
          (new-parents (cons ast parents)))
      (unless (and bounds
                   (or (< cursor-pos (car bounds))
                       (>= cursor-pos (cdr bounds))))
        (setf (getf (cddr ast) :position) child-pos)
        (or (leap-locate (second ast) cursor-pos nil new-parents)
            (when bounds
              new-parents))))))


(defun complete-command (match-set partial start end)
  (dolist (pair leap.parser:*function-names/alist*)
    (when (and (<= (length partial) (length (car pair)))
               (string-equal partial (subseq (car pair) 0 (length partial))))
      (jupyter:match-set-add match-set (car pair) start end :type "command"))))


(defmethod jupyter:complete-code ((k kernel) match-set code cursor-pos)
   (if (or (not *leap-syntax*)
           (lisp-code-p code))
    (call-next-method)
    (let* ((ast (ignore-errors (leap-read code)))
           (fragment (unless (typep ast 'jupyter:result)
                       (leap-locate ast (1- cursor-pos)))))
      (when (and fragment
                 (eql :s-expr (caar fragment)))
        (let ((start (car (getf (car fragment) :bounds)))
              (end (cdr (getf (car fragment) :bounds))))
          (call-next-method k (jupyter:make-offset-match-set :parent match-set :offset start)
                            (subseq code start end) (- cursor-pos start))))
      (when (and fragment
                 (eql :s-expr (caar fragment))
                 (symbolp (getf (car fragment) :value))
                 (eql 0 (getf (car fragment) :position))
                 (eql :instruction (caadr fragment)))
        (complete-command match-set (symbol-name (getf (car fragment) :value))
              (car (getf (car fragment) :bounds))
              (cdr (getf (car fragment) :bounds)))))))


(defclass cando-installer (jupyter:installer)
  ()
  (:default-initargs
    :class 'kernel
    :language +language+
    :display-name +display-name+
    :kernel-name +language+
    :systems '(:cando-jupyter)))

(defclass system-installer (jupyter:system-installer cando-installer)
  ()
  (:documentation "cando system installer."))

(defclass user-installer (jupyter:user-installer cando-installer)
  ()
  (:documentation "cando user installer."))

(defclass user-image-installer (jupyter:user-image-installer cando-installer)
  ()
  (:documentation "cando user image installer."))

(defmethod jupyter:command-line ((instance user-installer))
  "Get the command line for a user installation."
  (let ((implementation (jupyter:installer-implementation instance)))
    (list
      (or implementation
          (first (uiop:raw-command-line-arguments))
          (format nil "~(~A~)" (uiop:implementation-type)))
      "-f" "no-auto-lparallel"
      "--eval" "(ql:quickload :cando-jupyter)"
      "--eval" "(jupyter:run-kernel 'cando-jupyter:kernel #\"{connection_file}\")")))

(defmethod jupyter:command-line ((instance system-installer))
  "Get the command line for a system installation."
  (let ((implementation (jupyter:installer-implementation instance)))
    (list
      (or implementation
          (first (uiop:raw-command-line-arguments))
          (format nil "~(~A~)" (uiop:implementation-type)))
      "-f" "no-auto-lparallel"
      "--load" (namestring (jupyter:installer-path instance :root :program :bundle))
      "--eval" "(asdf:load-system :cando-jupyter)"
      "--eval" "(jupyter:run-kernel 'cando-jupyter:kernel #\"{connection_file}\")")))

(defun install (&key bin-path system local prefix)
  "Install Cando kernel.
- `bin-path` specifies path to LISP binary.
- `system` toggles system versus user installation.
- `local` toggles `/usr/local/share versus` `/usr/share` for system installations.
- `prefix` key specifies directory prefix for packaging.
"
  (jupyter:install
    (make-instance
      (if system
        'system-installer
        'user-installer)
      :implementation bin-path
      :local local
      :prefix prefix)))

(defun install-image (&key prefix)
  "Install Cando kernel based on image.
- `prefix` key specifies directory prefix for packaging."
  (jupyter:install
    (make-instance 'user-image-installer
      :prefix prefix)))


(defun do-run-kernel-from-slime ()
  (jupyter:run-kernel 'cando-jupyter:kernel (core:argv (1- (core:argc)))))


(defun run-kernel-from-slime ()
  (bordeaux-threads:make-thread
   (lambda ()
     (loop for time from 1 below 100
           do (format t "Starting kernel for the ~a time~%" time)           
           do (jupyter:run-kernel 'cando-jupyter:kernel (core:argv (1- (core:argc))))))
   :name :jupyter))

(defun jupyterlab ()
  (run-kernel-from-slime))
