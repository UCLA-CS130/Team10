# build tar file
docker build -t webserver.build .
docker run webserver.build > binary.tar

# Create deployment directory
mkdir deploy/

# Add demo files and extract tar
cp -r demo/ deploy/
tar xvf binary.tar -C deploy/
rm -f binary.tar

# Create Dockerfile.run
touch deploy/Dockerfile
echo "FROM busybox:ubuntu-14.04

WORKDIR /opt/webserver
COPY . /opt/webserver

CMD [\"./webserver\", \"demo/config.conf\"]" > deploy/Dockerfile

# tar the directory
tar czf dist.tar.gz deploy

# remove directory when done
rm -rf deploy/

REMOTE="ec2-user@ec2-35-163-65-169.us-west-2.compute.amazonaws.com"

# scp
scp -i Admin.pem dist.tar.gz $REMOTE:~/.

# ssh
ssh -i Admin.pem $REMOTE bash -c "'

# stop webserver
docker ps -q --filter ancestor=webserver | xargs docker kill

# untar into webserver directory
rm -rf deploy
tar xzvf dist.tar.gz

# Build and run webserver from shrunken image
docker build -t webserver deploy
docker run --rm -t -p 80:80 webserver &

'"
