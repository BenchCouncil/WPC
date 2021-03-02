import java.lang.management.ManagementFactory;
import java.lang.management.OperatingSystemMXBean;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Random;

public class sha256 {
    static final int ONE_MILLION = 1000000;
    static final int ONE_BILLION = 1000000000;
    static final long ONE_GIGABYTE = 1073741824L;
    static final long ONE_MEGABYTE = 1048576L;
    private long fIterations = 25L;
    private String fName = "sha256";
    private boolean fVerbose = false;
    private int fArrayElementCount;
    private byte[] fHashMe;
    private MessageDigest fDigestSha256 = null;
    private Random fRNG;

    public sha256(boolean testMode) throws NoSuchAlgorithmException {
        this.fDigestSha256 = MessageDigest.getInstance("SHA-256");
        this.fArrayElementCount = ONE_MILLION;
        allocateArray();
        this.fVerbose = true;
    }

    public sha256() throws NoSuchAlgorithmException {
        this.fDigestSha256 = MessageDigest.getInstance("SHA-256");
        this.fArrayElementCount = ONE_MILLION;
        allocateArray();
    }

    public void setRNG(Random var1) {
        this.fRNG = var1;
        this.fRNG.setSeed(System.nanoTime());
        System.gc();
    }

    private void allocateArray() {
        this.fHashMe = new byte[this.fArrayElementCount];
    }

    public void perturbArray() {
        for(int i = 0; i < 100; ++i) {
            int idx = this.fRNG.nextInt(this.fArrayElementCount);
            byte val = (byte)this.fRNG.nextInt(255);
            this.fHashMe[idx] = val;
        }
    }

    private void initializeArray() {
        this.fRNG.nextBytes(this.fHashMe);
    }

    private void verboseOut(String var1) {
        if (this.fVerbose) {
            System.out.println(var1);
        }
    }

    public void releaseArrays() {
        this.verboseOut("Setting arrays to null...");
        this.fHashMe = null;
        this.verboseOut("Requesting garbage collection...");
        System.gc();
    }

    private double runIteration() {
        long tm_begin = System.nanoTime();
        this.fDigestSha256.reset();
        byte[] arr = this.fDigestSha256.digest(this.fHashMe);
        long tm_end = System.nanoTime();
        long elapsed_nanotime = tm_end - tm_begin;
        double elapsed_sec = (double)elapsed_nanotime / 1.0E9D;
        return elapsed_sec;
    }

    public double run() {
        double elapsed_sec = this.runIteration();
        this.verboseOut("Measurement time: " + elapsed_sec);
        this.verboseOut("Sha256 ArrayCount: " + fArrayElementCount);
        return elapsed_sec;
    }

    public static void main(String [] args) {
        int arrayElementCount = 20 * (1 << 20);
        int iterations = 100;
        if (args.length >= 1) {
            arrayElementCount = Integer.parseInt(args[0]);
        }
        if (args.length >= 2) {
            iterations = Integer.parseInt(args[1]);
        }
        try{
            sha256 fSha256 = new sha256();
            Random fRNG = new Random();
            fSha256.setRNG(fRNG);
            fSha256.fArrayElementCount = arrayElementCount;
            fSha256.fIterations = iterations;
            fSha256.releaseArrays();
            fSha256.allocateArray();
            try {
                fSha256.initializeArray();
            }catch (java.lang.NullPointerException e){
                e.printStackTrace();
            }
            fSha256.perturbArray();
            double total_elapsed = 0;
            for(int count = 0; count < fSha256.fIterations; ++count) {
                total_elapsed += fSha256.run();
            }
            System.out.println("Array Element Count " + arrayElementCount);
            System.out.println("Iterations " + iterations);
            System.out.println("Total time wihout initialization " + total_elapsed);
            System.out.println("Throughtputs (Counts/s) " + fSha256.fIterations / total_elapsed);

        }catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
    }
}

