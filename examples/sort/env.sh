export HADOOP_HOME=/opt/benchcpu/hadoop-3.2.1-standalone
export HADOOP_CONF_DIR=$HADOOP_HOME/etc/hadoop
export HADOOP_MAPRED_HOME=$HADOOP_HOME
export HADOOP_COMMON_HOME=$HADOOP_HOME
export HADOOP_HDFS_HOME=$HADOOP_HOME
export YARN_HOME=$HADOOP_HOME
export HADOOP_COMMON_LIB_NATIVE_DIR=$HADOOP_HOME/lib/native
export HADOOP_OPTS="$HADOOP_OPTS -Djava.library.path=$HADOOP_HOME/lib/native"
export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64

HADOOP_CLASSPATH=$($HADOOP_HOME/bin/hadoop classpath)
export CLASSPATH=.:$HADOOP_CLASSPATH:$CLASSPATH


export PATH=$PATH:$HADOOP_HOME/bin:$HADOOP_HOME/sbin
