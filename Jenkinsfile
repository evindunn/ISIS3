// vim: ft=groovy

def labels = ["CentOS", "Fedora", "Mac", "Ubuntu"]
def nodes = [:]

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
