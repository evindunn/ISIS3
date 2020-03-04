// vim: ft=groovy

def labels = ["CentOS", "Fedora", "Mac", "Ubuntu"]
def labels = ["CentOS"]
def nodes = [:]
def errors = []

for (lbl in labels) {
    def label = lbl
    nodes[label] = {
         stage(label) {
            def nodeFailed = false;

            isisNode(label) {
                try {
                    error "ERROR!"

                } catch(e) {
                    errors.add("Failed on ${label}")           
                    nodeFailed = true;
                }

                if (nodeFailed) {
                    error "Failed on ${label}"
                }
            }
         }
    }
}

node {
    parallel nodes

    if (errors.size() > 0) {
        currentBuild.result = 'FAILURE';

        def comment = "Failed during:\n"
        errors.each {
            comment += "- ${it}\n"
        }

        setGithubStatus(comment)
    }
}
