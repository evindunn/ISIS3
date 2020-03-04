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
                    if (label != "Ubuntu") {
                        error "ERROR!"
                    }

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
    loginShell 'echo $(env)'

    try {
        parallel nodes

    } catch(e) {
        currentBuild.result = 'FAILURE';

        def comment = "Failed during:\n"
        errors.each {
            comment += "- ${it}\n"
        }
        
        println comment
        setGithubStatus(comment)
    }
}
