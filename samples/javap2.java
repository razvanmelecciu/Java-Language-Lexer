'\'';
class TestClass {
   public static void main(String[] args) {
       int a = 5;

       int[] arr = new int[8];
       char b = 'b';
        

        // this line will be ignored
        a / 5;

        if (a < b) {
            a++;
            return false;
        } else if (a <= b) {
            b+=7;
            return true;
        }

       return null /* inline comment */;
   }
}