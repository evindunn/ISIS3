// vim: set ft=groovy:

def numCores = 8

def cmakeFlags = [
    "-GNinja",
    "-DJP2KFLAG=ON",
    "-DKAKADU_INCLUDE_DIR=/isisData/kakadu",
    "-Dpybindings=OFF",
    "-DCMAKE_BUILD_TYPE=RELEASE"
]

def isisEnv = [
    "ISIS3DATA=/isisData/data",
    "ISIS3TESTDATA=/isisData/testData",
    "ISIS3MGRSCRIPTS=/isisData/data/isis3mgr_scripts",
]

def doEnviron(envFile) {
    sh """#!/bin/bash -l
        conda create -n isis3 python=3.6 || true
        conda activate isis3
        conda config --env --add channels conda-forge
        conda config --env --add channels usgs-astrogeology
        conda env update -n isis3 -f ${envFile} --prune
    """ 
}

def doBuild(isisRoot) {
    
}

pipeline {
    agent none
    environment {
        ISIS3DATA = '/isisData/data'
        ISIS3TESTDATA = '/isisData/testData'
        ISIS3MGRSCRIPTS = '/isisData/data/isis3mgr_scripts'
    }
    stages {
        stage('Dispatch Jobs') {
            parallel {
                stage('CentOS') {
                    agent { label 'centos-test' }
                    steps {
                        container("centos") {
                            stages {
                                stage("Checkout") {
                                    steps {
                                        // Checkout / environment
                                        checkout scm

                                        doEnviron("environment_gcc4.yml")
                                        sh '''
                                            echo 'conda activate isis3' >> ~/.bashrc
                                            mkdir build install
                                        '''
                                    }
                                }

                                stage("Build") {
                                    environment {
                                        ISISROOT="${pwd}/build"
                                    }

                                    steps {
                                        dir(env.ISISROOT) {
                                            sh """#!/bin/bash -l
                                                cmake ${(cmakeFlags + ["-DCMAKE_INSTALL_PREFIX=${pwd()}/install"]).join(' ')} ../isis
                                                ninja -j${numCores} install 
                                            """
                                        }
                                    }
                                }
                            }
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
