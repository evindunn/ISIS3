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

def doBuild(label, isisEnv, cmakeFlags) {
    def envFile = (label == 'CentOS') ? "environment_gcc4.yml" : "environment.yml"

    // Checkout
    checkout scm
    sh 'mkdir build install'

    // Environment
    sh """#!/bin/bash -l
        conda create -n isis3 python=3.6 || true
        conda activate isis3
        conda config --env --add channels conda-forge
        conda config --env --add channels usgs-astrogeology
        conda env update -n isis3 -f ${envFile} --prune
        echo 'conda activate isis3' >> ~/.bashrc
        echo 'conda activate isis3' >> ~/.bash_profile
    """ 

    withEnv(isisEnv + ["ISISROOT=${pwd()}/build"]) {
        dir(env.ISISROOT) {
            try {
                // Build
                sh """#!/bin/bash -l
                    cmake ${(cmakeFlags + ["-DCMAKE_INSTALL_PREFIX=${pwd()}/install"]).join(' ')} ../isis
                    ninja -j${numCores} install
                """        
            } catch(e) {
                error "Error while building on ${label}"
            }
           
        }
    }        
}

def doTests(label, isisEnv) {
    def errors = []

    withEnv(isisEnv + ["ISISROOT=${pwd()}/build"]) {
        dir(env.ISISROOT) {
            try { 
                // Unit tests
                sh """#!/bin/bash -l
                    ctest -R _unit_ -j${numCores} -VV
                """
            } catch(e) {
                errors.add("- Error during unit tests on ${label}")
            }

            try { 
                // App tests
                sh """#!/bin/bash -l
                    ctest -R _app_ -j${numCores} -VV
                    ctest -R _module_ -j${numCores} -VV
                """
            } catch(e) {
                errors.add("- Error during app tests on ${label}")
            }

            try {
                // Gtests
                sh """#!/bin/bash -l
                    ctest -R "." -E "(_app_|_unit_|_module_)" -j${numCores} -VV
                """
            } catch(e) {
                errors.add("- Error during gtests on ${label}")
            }
        }
    }

    error errors.join('\n')
}

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

def numCores = 8
def errors = []
def labels = ["Centos", "Fedora", "Mac", "Ubuntu"]
def builders = [:]

for (lbl in labels) {
    def lblLower = lbl.toLowerCase()

    builders[lblLower] = {
        // Make sure node has a label named lblLower
        node("${lblLower}-test") {
            if (lblLower.equals("mac")) {
                cleanWs()
                try {
                    doBuild(lbl, isisEnv, cmakeFlags)
                    doTests(lbl, isisEnv)
                } catch(e) {
                    errors.add(e)
                }

            } else {
                // Make sure pod template has a container named lblLower
                container(lblLower) {
                    try {
                        doBuild(lbl, isisEnv, cmakeFlags)
                        doTests(lbl, isisEnv)
                    } catch(e) {
                        errors.add(e)
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
    if (errors.size() > 0) {
        currentBuild.result = "FAILURE"
        errors.each {
            comment += "${it}\n"
        }
    }
    println "${comment}"
    setGitHubBuildStatus(comment)
}
