public class Market {
    public static void main(String[] args) {
        Container c = new Container();
        Producer p1 = new Producer(c);
        Producer p2 = new Producer(c);
        p1.setPname("生产者1");
        p2.setPname("生产者2");

        Consumer c1 = new Consumer(c);
        Consumer c2 = new Consumer(c);
        c1.setCname("消费者1");
        c2.setCname("消费者2");

        p1.start();
        p2.start();
        c1.start();
        c2.start();
    }
}

class Producer extends Thread {
    private String name;
    private Container container;
    public Producer(Container container) {
        this.container = container;
    }
    public void setPname(String name){
        this.name = name;
    }
    @Override
    public void run() {
        synchronized (container) {
            for (int i = 0; i < 50; i++) {
                Good good = new Good(i + 1, name);
                container.produce(good);
                System.out.println(name + "生产了第" + (i + 1) + "个产品");
            }
        }
    }
}

class Consumer extends Thread{
    private String name;
    private Container container;

    public Consumer(Container container) {
        this.container = container;
    }
    public void setCname(String name){
        this.name = name;
    }
    @Override
    public void run() {
        synchronized (container) {
            for (int i = 0; i < 50; i++) {
                Good good = container.purchase();
                System.out.println(name + "购买了" + good.name + "生产的" + "第" + good.id + "个产品");
            }
        }
    }
}

class Container {
    Good[] container = new Good[30];
    int count = 0;
    public void produce(Good good)  {
        //如果容器已满，则通知消费者消费
        if(count>=container.length){
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        //如果容器未满，则先生产再通知消费者消费
        container[count] = good;
        count++;
        this.notifyAll();
    }

    public Good purchase() {
        //如果容器中没有产品，则通知生产者生产
        if (count<=0){
            try {
                this.wait(2000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        //如果容器中有产品，则先消费再通知生产者生产
        count--;
        Good good = container[count];
        this.notifyAll();
        return good;
    }
}

class Good{
    int id;
    String name;
    public Good(int id,String name) {
        this.id = id;
        this.name = name;
    }
}