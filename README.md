# LearnDocker
git clone https://github.com/zihasyu/LearnDocker.git
cd LearnDocker
# please open docker desktop yourself
docker build -t cache-server .
docker-compose up -d

cd script
./sdcs-test.sh 3