import java.util.*;
import java.io.*;
import org.apache.hadoop.util.GenericOptionsParser;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;


public class sort_mr {
    static final int RAND_MAX = 2147483647;
    public static class Map extends 
        Mapper<Object, Text, IntWritable, IntWritable> {
        private static IntWritable data = new IntWritable();
        public void map(Object key, Text value, Context context)
            throws IOException, InterruptedException {
            String line = value.toString();
            String[] split = line.split("\\s+");
            if(split.length > 1) return;
            data.set(Integer.parseInt(line));
            context.write(data, new IntWritable(1));
        }
    }

    public static class Reduce extends
        Reducer<IntWritable, IntWritable, IntWritable, IntWritable> {
        private static IntWritable linenum = new IntWritable(1);
        public void reduce(IntWritable key, Iterable<IntWritable> values,
                Context context)
            throws IOException, InterruptedException {
            for (IntWritable val : values) {
                context.write(linenum, key);
                linenum = new IntWritable(linenum.get() + 1);
            }
        }
    }

    public static void main(String[] args) throws IOException, InterruptedException, ClassNotFoundException {
        Configuration conf = new Configuration();
        // conf.set("mapred.job.tracker", "localhost:9001");

        String[] ioArgs = new String[] {"input", "output" };
        String[] otherArgs = new GenericOptionsParser(conf, ioArgs).getRemainingArgs();
        
        File output_dir = new File(otherArgs[1]);
        if (output_dir.exists() && output_dir.isDirectory()) {
            File[] listFiles = output_dir.listFiles();
            for(File file : listFiles){
                System.out.println("Deleting " + file.getName());
                file.delete();
            }
            System.out.println("Deleting Directory. Success = " + output_dir.delete());
        }

        if (otherArgs.length != 2) {
            System.err.println("Usage: Data Sort <in> <out>");
            System.exit(2);
        }

        Job job = new Job(conf, "Data Sort");
        job.setJarByClass(sort_mr.class);

        job.setMapperClass(Map.class);
        job.setReducerClass(Reduce.class);

        job.setOutputKeyClass(IntWritable.class);
        job.setOutputValueClass(IntWritable.class);

        FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
        FileOutputFormat.setOutputPath(job, new Path(otherArgs[1]));
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}