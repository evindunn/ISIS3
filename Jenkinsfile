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
                        container("centos") {
                            // Checkout / environment
                            checkout scm
                            doEnviron("environment_gcc4.yml")
                            sh "echo 'conda activate isis3' >> ~/.bashrc"

                            // Build
                            
                        }
                    }
                }

                stage("Fedora") {
                    agent { label "fedora-test" }
                    steps {
                        sh 'echo Success!'
                    }
                }
                
                stage("Mac") {
                    agent { label "mac-test" }
                    steps {
                        sh 'echo Success!'
                    }
                }
                
                stage("Ubuntu") {
                    agent { label "ubuntu-test" }
                    steps {
                        sh 'echo Success!'
                    }
                }
            }
        }
    }
}
