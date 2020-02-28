// vim: ft=groovy

// Helpers for setting commit status
def getRepoUrl() {
    return sh(script: "git config --get remote.origin.url", returnStdout: true).trim()
}

def getCommitSha() {
    return sh(script: "git rev-parse HEAD", returnStdout: true).trim()
}


def setGitHubBuildStatus(status) {
    def repoUrl = getRepoUrl()
    def commitSha = getCommitSha()

    step([
        $class: 'GitHubCommitStatusSetter',
        reposSource: [$class: "ManuallyEnteredRepositorySource", url: repoUrl],
        commitShaSource: [$class: "ManuallyEnteredShaSource", sha: commitSha],
        errorHandlers: [[$class: 'ShallowAnyErrorHandler']],
        statusResultSource: [
          $class: 'ConditionalStatusResultSource',
          results: [
            [$class: 'BetterThanOrEqualBuildResult', result: 'SUCCESS', state: 'SUCCESS', message: status],
            [$class: 'BetterThanOrEqualBuildResult', result: 'FAILURE', state: 'FAILURE', message: status],
            [$class: 'AnyBuildResult', state: 'FAILURE', message: 'Loophole']
          ]
        ]
    ])
}

pipeline {
  agent any
  stages {
    stage("CI") {
      steps { 
        script {
          def groovy_utilities = load "${pwd()}/groovy_utilities.groovy"  

          def errors = []
          def labels = ['CentOS', 'Fedora', 'Ubuntu', 'Mac'] // labels for Jenkins node types we will build on
          def builders = [:] 
          
          println("Using Labels: " + labels)

          for (x in labels) {
            def label = x
            def lower_label = label.toLowerCase()
        
            builders[lower_label] = {
              node("${lower_label}-test") {
              
              def build_ok = true
              def condaPath = ""
              def isisEnv = [
                "ISIS3DATA=/isisData/data",
                "ISIS3TESTDATA=/isisData/testData",
                "ISIS3MGRSCRIPTS=/isisData/data/isis3mgr_scripts",
                "MALLOC_CHECK_=1"
              ]

              def cmakeFlags = [
                  "-DJP2KFLAG=ON",
                  "-DKAKADU_INCLUDE_DIR=/isisData/kakadu",
                  "-Dpybindings=OFF",
                  "-DCMAKE_BUILD_TYPE=RELEASE"
              ]

              def envFile = (lower_label == "centos") ? "environment_gcc4.yml" : "environment.yml"

              stage (label) {
                  container(lower_label) {
                      checkout scm
                      isisEnv.add("ISISROOT=${pwd()}/build")
                      cmakeFlags.add("-DCMAKE_INSTALL_PREFIX=${pwd()}/install")
                          
                      env.STAGE_STATUS = "Creating conda environment"

                      withEnv(isisEnv) {
                        sh """#!/bin/bash -l
                            conda create -c conda-forge -n isis3 python=3 || true
                            conda env update -n isis3 -f ${envFile} --prune
                        """

                        dir("${env.ISISROOT}") {
                            try {
                                  env.STAGE_STATUS = "Building ISIS on ${label}"
                                  sh """#!/bin/bash -l
                                      conda activate isis3
                                      cmake -GNinja ${cmakeFlags.join(' ')} ../isis
                                      ninja -j4 install
                                  """
                            }
                            catch(e) {
                                build_ok = false
                                errors.add(env.STAGE_STATUS)
                                println e.toString()
                            }

                            if (build_ok) {
                                try{
                                    dir("${env.ISISROOT}") {
                                        env.STAGE_STATUS = "Running unit tests on ${label}"
                                        sh """#!/bin/bash -l
                                            conda activate isis3
                                            ctest -R _unit_ -j4 -VV
                                        """
                                    }
                                }
                                catch(e) {
                                    build_ok = false
                                    errors.add(env.STAGE_STATUS)
                                    echo e.toString()
                                }

                                try{
                                    env.STAGE_STATUS = "Running app tests on ${label}"
                                    sh """#!/bin/bash -l
                                        conda activate isis3
                                        ctest -R _app_ -j4 -VV
                                    """
                                }
                                catch(e) {
                                    build_ok = false
                                    errors.add(env.STAGE_STATUS)
                                    println e.toString()
                                }

                                try{
                                    sh """#!/bin/bash -l
                                        conda activate isis3
                                        ctest -R _module_ -j4 -VV
                                    """
                                }
                                catch(e) {
                                    build_ok = false
                                    errors.add(env.STAGE_STATUS)
                                    println e.toString()
                                }

                                try{
                                    env.STAGE_STATUS = "Running gtests on ${label}"
                                    sh """#!/bin/bash -l
                                        conda activate isis3
                                        ctest -R "." -E "(_app_|_unit_|_module_)" -j4 -VV
                                    """
                                }
                                catch(e) {
                                    build_ok = false
                                    errors.add(env.STAGE_STATUS)
                                    println e.toString()
                                }
                            }
                          }
                          
                          if(build_ok) {
                              currentBuild.result = "SUCCESS"
                          }
                          else {
                              currentBuild.result = "FAILURE"
                              def comment = "Failed during:\n"
                              errors.each {
                                  comment += "- ${it}\n"
                              }
                              setGitHubBuildStatus(comment)
                          }
                      }
                    }
                }
              }
            }
          }
          parallel builders
        }
      }
    }
  }
}
