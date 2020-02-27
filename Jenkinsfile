// vim: set ft=groovy:

def doEnviron(envFile) {
    sh """#!/bin/bash -l
        conda create -n isis3 python=3.6 || true
        conda activate isis3
        conda config --env --add channels conda-forge
        conda config --env --add channels usgs-astrogeology
        conda env update -n isis3 -f ${envFile} --prune
    """ 
}

pipeline {
    agent none
    stages {
        stage("Dispatch Jobs") {
            parallel {
                stage("CentOS") {
                    agent { label "centos-test" }

                    steps {
                        checkout scm
                        doEnviron("environment_gcc4.yml")         
                    }
                }

                stage("Fedora") {
                    agent { label "fedora-test" }

                    steps {
                        checkout scm
                        doEnviron("environment.yml")         
                    }
                }
            }
        }
    }
}
