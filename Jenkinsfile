// vim: ft=groovy

// def labels = ["CentOS", "Fedora", "Mac", "Ubuntu"]
def labels = ["CentOS"]
def nodes = [:]

for (lbl in labels) {
    def label = lbl
    nodes[label] = {
         stage(label) {
            isisNode(label) {
                condaEnv("isis3") {
                    sh 'env'
                }

                condaEnv("isis3") {
                    checkout scm
                    loginShell "conda env update -f environment.yml"
                }
            }
         }
    }
}

parallel nodes
