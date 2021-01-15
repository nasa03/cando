FROM ubuntu:20.10

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y build-essential python3-pip npm git sbcl wget libzmq3-dev

RUN pip3 install jupyterlab
RUN jupyter-labextension install \
     @jupyter-widgets/jupyterlab-manager cytoscape-clj ipysheet \
     kekule-clj nglview-js-widgets@2.7.7 \
     ngl-clj resizable-box-clj

# Set up the environment
ARG APP_USER=app
ARG APP_UID=1000
ENV USER ${APP_USER}
ENV HOME /home/${APP_USER}
ENV PATH "$HOME/.local/bin:$PATH"
ENV SLIME_HOME "${HOME}/quicklisp/local-projects/slime"
ENV AMBERHOME /opt/amber/

# Create a user
RUN useradd --create-home --shell=/bin/bash --uid=${APP_UID} ${APP_USER}
COPY --chown=${APP_UID}:${APP_USER} home ${HOME}

WORKDIR ${HOME}
USER ${APP_USER}

# Install quicklisp
RUN wget --no-check-certificate https://beta.quicklisp.org/quicklisp.lisp && \
    sbcl --non-interactive --load quicklisp.lisp --eval "(quicklisp-quickstart:install)" && \
    rm quicklisp.lisp

RUN sbcl --non-interactive --eval '(ql-dist:install-dist "http://thirdlaw.tech/quickclasp/quickclasp.txt" :prompt nil)'

# Use SBCL to install needed packages and then install all of the kernels
RUN sbcl --non-interactive --eval "(ql:quickload '(:common-lisp-jupyter :kekule-clj :cytoscape-clj :nglview-cl :ngl-clj))" --eval "(cl-jupyter:install :use-implementation t)"
     
