(in-package :cando-jupyter)

(defgeneric show (matter-or-trajectory &rest kwargs &key &allow-other-keys)
  (:documentation "Display a residue, molecule, aggregate or a trajectory in a Jupyter notebook"))



(defun add-bounding-box (widget aggregate)
  (unless (chem:bounding-box-bound-p aggregate)
    (error "There is no bounding-box defined for aggregate"))
  (let* ((bounding-box (chem:bounding-box aggregate))
         (center (chem:get-bounding-box-center bounding-box))
         (x-width/2 (/ (chem:get-x-width bounding-box) 2.0))
         (y-width/2 (/ (chem:get-y-width bounding-box) 2.0))
         (z-width/2 (/ (chem:get-z-width bounding-box) 2.0))
         (minx (- (geom:vx center) x-width/2))
         (miny (- (geom:vy center) y-width/2))
         (minz (- (geom:vz center) z-width/2))
         (maxx (+ (geom:vx center) x-width/2))
         (maxy (+ (geom:vy center) y-width/2))
         (maxz (+ (geom:vz center) z-width/2))
         (xpair (vector minx maxx))
         (ypair (vector miny maxy))
         (zpair (vector minz maxz)))
    (flet ((line (from to &optional (color #(0 0 0)))
             (let ((start (vector (elt xpair (elt from 0)) (elt ypair (elt from 1)) (elt zpair (elt from 2))))
                   (stop  (vector (elt xpair (elt to 0)) (elt ypair (elt to 1)) (elt zpair (elt to 2)))))
               (vector "arrow" start stop color 1.0))))
      (nglv::add-shape widget (vector (line #(0 0 0) #(1 0 0) #(1 0 0))
                                      (line #(0 0 0) #(0 1 0) #(0 1 0))
                                      (line #(0 0 0) #(0 0 1) #(0 0 1))
                                      (line #(1 0 0) #(1 1 0))
                                      (line #(1 0 0) #(1 0 1))
                                      (line #(0 1 0) #(1 1 0))
                                      (line #(0 1 0) #(0 1 1))
                                      (line #(0 0 1) #(1 0 1))
                                      (line #(0 0 1) #(0 1 1))
                                      (line #(1 1 0) #(1 1 1))
                                      (line #(1 0 1) #(1 1 1))
                                      (line #(0 1 1) #(1 1 1)))
                      :name "bounding-box"))))

(defmethod show ((aggregate chem:aggregate) &rest kwargs &key &allow-other-keys)
  (let ((structure (make-instance 'cando-structure :matter aggregate))
        (axes (getf kwargs :axes))
        (shapes (getf kwargs :shapes)))
    (remf kwargs :axes)
    (remf kwargs :shapes)
    (let ((widget (apply #'nglv:make-nglwidget :structure structure kwargs)))
      (when (chem:bounding-box-bound-p aggregate)
        (add-bounding-box widget aggregate))
      (when axes (nglv:add-axes widget))
      (when shapes
        (loop for index below (length shapes) by 2
              for name = (string (elt shapes index))
              for shape = (elt shapes (1+ index))
              do (nglv::add-shape widget shape :name name)))
      widget)))

(defmethod show ((molecule chem:molecule) &rest kwargs &key &allow-other-keys)
  (let ((agg (chem:make-aggregate nil)))
    (chem:add-matter agg molecule)
    (apply 'show agg kwargs)))

(defun isolate-residue (residue)
  (let ((agg (chem:make-aggregate nil))
        (mol (chem:make-molecule nil))
        (res (chem:copy residue)))
    (let (break-bonds)
      (chem:map-bonds
       'nil
       (lambda (a1 a2 order)
         (format t "Looking at ~a ~a~%" a1 a2)
         (unless (and (chem:contains-atom res a1) (chem:contains-atom res a2))
           (format t "Break bond ~a ~a~%" a1 a2)
           (push (list a1 a2) break-bonds)))
       res)
      (loop for bond in break-bonds
            for a1 = (first bond)
            for a2 = (second bond)
            do (chem:remove-bond-to a1 a2)))
    (chem:add-matter agg mol)
    (chem:add-matter mol res)
    agg))

(defmethod show ((residue chem:residue)  &rest kwargs &key &allow-other-keys)
  (let ((agg (isolate-residue residue)))
    (apply 'show agg kwargs)))
  
(defun repr (widget representation &optional (selection "all"))
  (funcall (find-symbol "ADD-REPRESENTATION" :nglv) widget representation :selection selection))

(defun cl-jupyter-kernel-start (&optional connection-file-name)
  (let ((amber-home
          (namestring (or (if (ext:getenv "AMBERHOME")
                              (probe-file (ext:getenv "AMBERHOME"))
                              "/usr/local/amber/")
                          (probe-file "/home/app/amber16-data/")
                          "/dev/null"))))
    (setf (logical-pathname-translations "amber")
          (list (list "**;*.*" (concatenate 'string amber-home "/**/*.*"))))
    (format t "Setting amber host pathname translation -> ~a~%" amber-home))
  (let ((*readtable* (copy-readtable)))
    (let ((cl-jup (find-symbol "KERNEL-START" "CL-JUPYTER")))
      (if cl-jup
          (if connection-file-name
              (funcall cl-jup connection-file-name)
              (funcall cl-jup))
          (error "cl-jupyter is not installed")))))



(defmethod show ((sketch sketch2d:sketch2d) &rest kwargs &key &allow-other-keys)
  (show (sketch2d:svg sketch)))

(defmethod show ((sketch sketch2d:sketch-svg) &rest kwargs &key &allow-other-keys)
  (cl-jupyter-user:svg (sketch2d:render-svg-to-string sketch)))

(defmethod show ((trajectory dynamics:trajectory) &rest kwargs &key &allow-other-keys)
  (change-class trajectory 'cando-trajectory)
  (apply #'nglv:make-nglwidget :structure trajectory kwargs))

(defmethod show ((dynamics dynamics:simulation) &rest kwargs &key &allow-other-keys)
  (let ((trajectory (dynamics:make-trajectory dynamics)))
    (change-class trajectory 'cando-trajectory)
    (apply 'show trajectory kwargs)))

