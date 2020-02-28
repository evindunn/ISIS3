// vim: ft=groovy

def labels = ["CentOS", "Fedora", "Mac", "Ubuntu"]
def nodes = [:]

node {
    stage("Allocate Nodes") {
        for (lbl in labels) {
            nodes[lbl] = {
                stage(lbl) {
                    isisNode(lbl) {
                        loginShell 'conda --version'
                    }
                }
            }
        }

        parallel nodes
    }
}
