// vim: ft=groovy

pipeline 
{
    stages
    {
        stage("do stuff")
        {
        parallel
        {
            stage('CentOS')
            {
                build 'ISIS-Builds/CentOS'
            }
            stage('Fedora')
            {
                build 'ISIS-Builds/Fedora'
            }
            stage('Ubuntu')
            {
                build 'ISIS-Builds/Fedora'
            }
            stage('Mac')
            {
                label 'mac'
                echo "Foo"
                sleep 10
            }
        }}
    }
}
