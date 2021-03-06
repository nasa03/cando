;;; Copyright (c) 2019, Christian E. Schafmeister
;;; Published under the GPL 2.0.  See COPYING
;;;

(in-package :asdf-user)

(defsystem "structure-editor"
  :description "Structure editor widgets"
  :version "0.0.1"
  :author "Christian Schafmeister <meister@thirdlaw.tech>"
  :licence "Private"
  :depends-on (:kekule-clj :cando-widgets :resizable-box-clj)
  :serial t
;;;  :build-operation asdf:monolithic-compile-bundle-op
;;;  :build-pathname #P"/tmp/tirun.fasb"
  :components
  ((:file "packages")
   (:file "composer")
   (:file "structure-editor")
   ))

