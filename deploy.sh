# build tar file
docker build -t webserver.build .
docker run webserver.build > binary.tar

# TODO: Connect via SCP
# TODO: SCP the binary file over
# TODO: SCP the demo directory over
# TODO: SSH into the EC2 Container
# TODO: tar the binary.tar file
# TODO: Delete the binary file
# TODO: Create Dockerfile.run
# TODO: Build and run webserver

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

# scp


# Build and run webserver from shrunken image
# docker build -t webserver deploy
# docker run --rm -t -p 3000:3000 webserver
