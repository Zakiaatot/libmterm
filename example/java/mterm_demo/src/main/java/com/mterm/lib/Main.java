package com.mterm.lib;

import java.io.IOException;
import java.io.InputStream;
import java.util.concurrent.TimeUnit;

public class Main {
    public static void main(String[] args) throws InterruptedException {
        int id = JNI.createMtermDefault();
        Thread readThread = new Thread(() -> {
            JNI.setReadNonblockMterm(id);
            byte[] buffer = new byte[4096];
            int res;
            try {
                while (true) {
                    res = JNI.readMterm(id, buffer, buffer.length);
                    if (res > 0) {
                        System.out.write(buffer, 0, res);
                        System.out.flush();
                    }
                    TimeUnit.MILLISECONDS.sleep(1);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        });

        Thread writeThread = new Thread(() -> {
            try {
                InputStream stdin = System.in;
                byte[] buffer = new byte[4096];
                int res;
                while (true) {
                    res = stdin.read(buffer);
                    if (res > 0) {
                        JNI.writeMterm(id, buffer, res);
                    }
                    TimeUnit.MILLISECONDS.sleep(1);
                }
            } catch (IOException | InterruptedException e) {
                e.printStackTrace();
            }
        });

        readThread.start();
        writeThread.start();

        readThread.join();
        writeThread.join();
    }

}