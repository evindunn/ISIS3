// vim: ft=groovy

def labels = ["CentOS"]
def nodes = [:]

node {
    stage("Allocate Nodes") {
        for (label in labels) {
            nodes[label] = {
                isisNode(label) {
                    stage(label) {
                        loginShell 'conda --version'
                    }
                }
            }
        }

        parallel nodes
    }
}
