// vim: ft=groovy

def labels = ["CentOS", "Fedora", "Mac", "Ubuntu"]
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
                    currentBuild.result = 'FAILURE';
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

        println comment
        setGithubStatus(comment)
    }
}
