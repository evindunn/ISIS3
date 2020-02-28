// vim: ft=groovy

pipeline {
    agent { label "centos-test" }
    stages {
        stage("Testing") {
            steps { 
                container("centos") {
                    script {
                        loginShell "conda --version"
                    }
                }
            }
        }
    }
}
