FROM centos:7

# Create a user, that wil be the ideal default user
# Search for "Support Arbitrary User IDs" https://access.redhat.com/documentation/en-us/openshift_container_platform/3.6/pdf/creating_images/OpenShift_Container_Platform-3.6-Creating_Images-en-US.pdf
RUN groupadd -g 1001 nic && \
    useradd -g nic -u 500 nic && \
    usermod -a -G root nic && \
    yum install -y tar unzip iputils \
    yum install gdb \
    yum install gcc \
    yum install make \
    yum install sendmail \
    yum install mailx

USER 500

RUN mkdir -p /home/nic/life
WORKDIR /home/nic/life
ADD src ./
RUN make

