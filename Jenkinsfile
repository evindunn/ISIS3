// vim: ft=groovy

def labels = ["CentOS", "Fedora", "Mac", "Ubuntu"]
def nodes = [:]

node {
    stage("Allocate Nodes") {
        for (lbl in labels) {
            def label = lbl
            nodes[label] = {
                 stage(label) {
                    isisNode(lbl) {
                        loginShell 'conda --version'
                    }
                 }
            }
        }

        parallel nodes
    }
}
