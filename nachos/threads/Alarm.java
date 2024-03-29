package nachos.threads;

import java.util.Comparator;
import java.util.PriorityQueue;

import nachos.machine.*;

/**
 * Uses the hardware timer to provide preemption, and to allow threads to sleep
 * until a certain time.
 */
public class Alarm {
    /**
     * Allocate a new Alarm. Set the machine's timer interrupt handler to this
     * alarm's callback.
     *
     * <p>
     * <b>Note</b>: Nachos will not function correctly with more than one alarm.
     */
    public Alarm() {
        Machine.timer().setInterruptHandler(new Runnable() {
            public void run() {
                timerInterrupt();
            }
        });
    }

    /**
     * The timer interrupt handler. This is called by the machine's timer
     * periodically (approximately every 500 clock ticks). Causes the current thread
     * to yield, forcing a context switch if there is another thread that should be
     * run.
     */
    public void timerInterrupt() {
        boolean initStatus = Machine.interrupt().disable();

        long currentTime = Machine.timer().getTime();
        while (!waitQueue.isEmpty() && waitQueue.peek().wakeTime <= currentTime) {
            KThread thread = waitQueue.poll().thread;
            thread.ready();
        }

        Machine.interrupt().restore(initStatus);

        KThread.yield();
    }

    /**
     * Put the current thread to sleep for at least <i>x</i> ticks, waking it up in
     * the timer interrupt handler. The thread must be woken up (placed in the
     * scheduler ready set) during the first timer interrupt where
     *
     * <p>
     * <blockquote> (current time) >= (WaitUntil called time)+(x) </blockquote>
     *
     * @param x the minimum number of clock ticks to wait.
     *
     * @see nachos.machine.Timer#getTime()
     */
    public void waitUntil(long x) {
        // for now, cheat just to get something working (busy waiting is bad)
        long wakeTime = Machine.timer().getTime() + x;

        boolean initStatus = Machine.interrupt().disable();

        waitQueue.offer(new WaitThread(KThread.currentThread(), wakeTime));
        KThread.sleep();

        Machine.interrupt().restore(initStatus);
    }

    public class WaitThread {
        public KThread thread;
        public long wakeTime;

        public WaitThread(KThread thread, long wakeTime) {
            this.thread = thread;
            this.wakeTime = wakeTime;
        }
    }

    private PriorityQueue<WaitThread> waitQueue = new PriorityQueue<>(new Comparator<WaitThread>() {
        public int compare(WaitThread a, WaitThread b) {
            if (a.wakeTime < b.wakeTime)
                return -1;
            else if (a.wakeTime > b.wakeTime)
                return 1;
            else
                return 0;
        }
    });

    static int fin = 0;

    public static void selfTest() {
        KThread thread1 = new KThread(new Runnable() {
            public void run() {
                ThreadedKernel.alarm.waitUntil(1000);
                fin = 1;
            }
        }).setName("wait1000");
        KThread thread2 = new KThread(new Runnable() {
            public void run() {
                ThreadedKernel.alarm.waitUntil(2000);
                if (fin == 0)
                    System.out.println("error");
            }
        }).setName("wait2000");
        thread2.fork();
        thread1.fork();
        thread2.join();
        System.out.println("\nAlarm test finish.");
    }
}
