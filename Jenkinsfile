// vim: ft=groovy

def labels = ["CentOS"]
def nodes = [:]

for (label in labels) {
    nodes[label] = {
        node(label) {
            stage(label) {
                loginShell 'conda --version'
            }
        }
    }
}

parallel nodes
