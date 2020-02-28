// vim: ft=groovy

def labels = ["CentOS", "Fedora", "Mac", "Ubuntu"]
def nodes = [:]

node {
    stage("Allocate Nodes") {
        for (lbl in labels) {
            nodes[lbl] = {
                isisNode(lbl) {
                    stage(lbl) {
                        loginShell 'conda --version'
                    }
                }
            }
        }

        parallel nodes
    }
}
