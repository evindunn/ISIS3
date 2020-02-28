// vim: ft=groovy

def labels = ["CentOS", "Fedora", "Mac", "Ubuntu"]
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
