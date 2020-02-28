// vim: ft=groovy

def labels = ["CentOS", "Fedora", "Mac", "Ubuntu"]
def nodes = [:]

for (lbl in labels) {
    def label = lbl
    nodes[label] = {
         stage(label) {
            isisNode(label) {
                loginShell 'conda --version'
            }
         }
    }
}

parallel nodes
