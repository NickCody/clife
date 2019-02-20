FROM centos:7

# Create a user, that wil be the ideal default user
# Search for "Support Arbitrary User IDs" https://access.redhat.com/documentation/en-us/openshift_container_platform/3.6/pdf/creating_images/OpenShift_Container_Platform-3.6-Creating_Images-en-US.pdf
RUN groupadd -g 1001 nic && \
    useradd -g nic -u 500 nic && \
    usermod -a -G root nic && \
    yum install -y tar unzip iputils \
    yum install gdb \
    yum install gcc \
    yum install make

WORKDIR /home/nic

# When images extend this one, they can use 'USER root' command again
# to allow for modifications that need privileges during build
# but they should always set back the USER to 500 just as a preferred practice
# (ideally they can run as any UID, also they should set the permissions on files to solidfy it as ready only for things that should not change)
USER 500


