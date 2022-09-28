import java.util.HashMap;
import java.util.Random;

public class Test {
    public static void main(String[] args) {
        int[] nums = new int[10000000];
        Random rd = new Random();
        for (int i = 0; i < 10000000; i++) {
            nums[i] = rd.nextInt();
        }
        HashMap<Integer, Integer> hashMap = new HashMap<>();
        long start = System.currentTimeMillis();
        for (int i = 0; i < 10000000; i++) 
        {
            hashMap.put(nums[i], nums[i]);
        }
        long timeTaken = System.currentTimeMillis() - start;
        System.out.println(timeTaken + "ms");    
    }
}

