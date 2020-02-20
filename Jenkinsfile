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

def numCores = 8
def errors = []
def labels = ["Centos", "Fedora", "Mac", "Ubuntu"]
def builders = [:]

for (lbl in labels) {
    def lblLower = lbl.toLowerCase()
    def envFile = (lbl == 'CentOS') ? "environment_gcc4.yml" : "environment.yml"

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

    builders[lblLower] = {
        // Make sure node has a label named lblLower
        node("${lblLower}-test") {
            if (lblLower.equals("mac")) {
                cleanWs()

            } else {
                // Make sure pod template has a container named lblLower
                container(lblLower) {
                    // Checkout
                    env.STAGE_STATUS = "Checkout on ${env.STAGE_NAME}"
                    checkout scm
                    sh 'mkdir build install'
                    isisEnv.add("ISISROOT=${pwd()}/build")
                    cmakeFlags.add("-DCMAKE_INSTALL_PREFIX=${pwd()}/install")

                    // Environment
                    env.STAGE_STATUS = "Environment on ${env.STAGE_NAME}"
                    sh """#!/bin/bash -l
                        conda create -n isis3 python=3.6 || true
                        conda activate isis3
                        conda config --env --add channels conda-forge
                        conda config --env --add channels usgs-astrogeology
                        conda env update -n isis3 -f ${envFile} --prune
                        echo 'conda activate isis3' >> ~/.bashrc
                    """ 

                    withEnv(isisEnv) {
                        dir(env.ISISROOT) {
                            try {
                                // Build
                                env.STAGE_STATUS = "Build on ${env.STAGE_NAME}"
                                sh """#!/bin/bash -l
                                    cmake ${cmakeFlags.join(' ')} ../isis
                                    ninja -j${numCores} install
                                """        
                            } catch(e) {
                                errors.add(env.STAGE_STATUS)
                                error env.STAGE_STATUS
                            }
                           
                            try { 
                                // Unit tests
                                env.STAGE_STATUS = "Unit tests on ${env.STAGE_NAME}"
                                sh """#!/bin/bash -l
                                    ctest -R _unit_ -j${numCores} -VV
                                """
                            } catch(e) {
                                errors.add(env.STAGE_STATUS)
                            }

                            try { 
                                // App tests
                                env.STAGE_STATUS = "App tests on ${env.STAGE_NAME}"
                                sh """#!/bin/bash -l
                                    ctest -R _app_ -j${numCores} -VV
                                    ctest -R _module_ -j${numCores} -VV
                                """
                            } catch(e) {
                                errors.add(env.STAGE_STATUS)
                            }

                            try {
                                // Gtests
                                env.STAGE_STATUS = "Gtests tests on ${env.STAGE_NAME}"
                                sh """#!/bin/bash -l
                                    ctest -R "." -E "(_app_|_unit_|_module_)" -j${numCores} -VV
                                """
                            } catch(e) {
                                errors.add(env.STAGE_STATUS)
                            }
                        }
                    }        
                }
            }
        }
    }
}

node {
    parallel builders

    // Populate result
    def comment = "All checks passed"
    if (errors.length > 0) {
        currentBuild.result = "FAILURE"
        comment = "Failed during:\n"
        errors.each {
            comment += "- ${it}\n"
        }
    }
    println "${comment}"
    setGitHubBuildStatus(comment)
}
