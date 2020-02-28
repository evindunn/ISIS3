// vim: ft=groovy

def labels = ["CentOS", "Mac"]
def nodes = [:]

for (label in labels) {
    println "Found label ${label}"
    nodes[label] = {
        isisNode(label) {
            stage(label) {
                loginShell 'conda --version'
            }
        }
    }
}

parallel nodes
