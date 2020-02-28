// vim: ft=groovy


def labels = ["CentOS", "Mac"]
def nodes = [:]

for (label in labels) {
    nodes[label] = {
        isisNode(label) {
            loginShell 'conda --version'
        }
    }
}

node {
    parallel nodes
}
