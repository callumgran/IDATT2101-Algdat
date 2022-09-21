import java.util.Scanner;
import static java.lang.Math.pow;

public class StringTree {

    public static final int LINE_WIDTH = (1 * 64);
    public static final int PRINT_HEIGHT = 4;
    public static final int SPACE_BETWEEN_LEVELS = 1;
    public static final int QUEUE_SIZE = (int) pow(2, PRINT_HEIGHT);

    public static void main(String[] args) {
        StringTree st = new StringTree();
        st.start();
    }

    public void start() {
        System.out.println("Enter word separated by spaces");
        System.out.println("To exit, press enter\n");
        Scanner sc = new Scanner(System.in);
        String input = sc.nextLine();
        String[] split = input.trim().split("\\s+");
        if (split.length > 0) {
            Tree tree = new Tree();
            for (String string : split) {
                Element item = new Element(string);
                tree.addToTree(item);
            }
            tree.levelOrder(new Handler());
        } else {
            System.out.println("No words in input");
        }
        sc.close();
    }

    class Element {
        private String element;

        public Element(String element) {
            this.element = element;
        }

        public String findKey() {
            return element;
        }
    }

    class Handler {
        private int level = 0,
                    nodes = 0;

        private String out = "";

        public Handler(){};

        public void nodeToOutput(Object e) {
            TreeNode node = (TreeNode) e;
            Element elem = (Element) node.element;
            if (elem != null && level < PRINT_HEIGHT) {
                String word = (String) elem.findKey();
                int halfLength = word.length() / 2,
                    whitespace = 0,
                    i = 0;
                if (word.length() % 2 != 0) {
                    i = 1;
                }
                whitespace = (int) ((LINE_WIDTH / pow(2, level + 1)) - halfLength);

                while (i < whitespace) {
                    out += " ";
                    i++;
                }

                out += word;

                while (i < 2 * whitespace) {
                    out += " ";
                    i++;
                }
                nodes++;
                if (nodes == pow(2, level)) {
                    System.out.println(out);
                    out = "";
                    nodes = 0;
                    level += 1;
                }
                
                Element emptyElement = new Element("");
                if (node.right == null)
                    node.right = new TreeNode(emptyElement, node, null, null);
                if (node.left == null)
                    node.left = new TreeNode(emptyElement, node, null, null);
            }
        }
    }

    class Queue {
        private Object[] tab;
        private int start = 0,
                    end = 0,
                    size = 0;

        public Queue(int size) {
            tab = new Object[size];
        }

        public boolean empty() {
            return size == 0;
        }

        public boolean full() {
            return size == tab.length;
        }

        public void addQueueItem(Object e) {
            if (full()) return;
            tab[end] = e;
            end = (end + 1) % tab.length;
            ++size;
        }

        public Object nextInQueue() {
            if (!empty()) {
                Object e = tab[start];
                start = (start + 1) % tab.length;
                --size;
                return e;
            } else return null;
        }

        public Object checkQueue() {
            if (!empty()) return tab[start];
            else return null;
        }
    }

    class TreeNode {
        Object element;
        TreeNode left, 
                right,
                parent;

        public TreeNode(Object element, TreeNode parent, TreeNode left, TreeNode right) {
            this.element = element;
            this.parent = parent;
            this.left = left;
            this.right = right;
        }
    }

    class Tree {
        TreeNode root;

        public Tree() {
            root = null;
        }

        public boolean empty() {
            return root == null;
        }

        public void levelOrder(Handler h) {
            Queue queue = new Queue(QUEUE_SIZE);
            queue.addQueueItem(root);
            while (!queue.empty()) {
                TreeNode curr = (TreeNode) (queue.nextInQueue());
                h.nodeToOutput(curr);
                if (curr.left != null) queue.addQueueItem(curr.left);
                if (curr.right != null) queue.addQueueItem(curr.right);
            }
        }

        public void addToTree(Object e) {
            if (root == null) {
                root = new TreeNode(e, null, null, null);
                return;
            }
            TreeNode node = root,
                    parent = null;
            Element elem = (Element) e,
                    compare = null;
            while (node != null) {
                parent = node;
                compare = (Element) node.element;
                if (elem.findKey().compareToIgnoreCase(compare.findKey()) <= 1) node = node.left;
                else node = node.right;
            }
            if (elem.findKey().compareToIgnoreCase(compare.findKey()) <= 1)
                parent.left = new TreeNode(elem, parent, null, null);
            else parent.right = new TreeNode(elem, parent, null, null);
        }
    }
}