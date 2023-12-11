package com.mterm.app;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.mterm.lib.JNI;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.concurrent.TimeUnit;

import com.google.android.material.textfield.TextInputEditText;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView out=(TextView)findViewById(R.id.out);
        @SuppressLint({"MissingInflatedId", "LocalSuppress"}) TextInputEditText in = (TextInputEditText)findViewById(R.id.in);
        @SuppressLint({"MissingInflatedId", "LocalSuppress"}) Button send = (Button) findViewById(R.id.send);
        int id = JNI.createMtermDefault();
        ByteArrayOutputStream outputStream = new ByteArrayOutputStream();


        send.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String str=in.getText().toString()+'\n';
                byte[] data = str.getBytes(StandardCharsets.UTF_8);
                JNI.writeMterm(id,data,data.length);
                in.setText("");
            }
        });

        Thread readThread = new Thread(() -> {
            JNI.setReadNonblockMterm(id);
            byte[] buffer = new byte[4096];
            int res;
            try {
                while (true) {
                    res = JNI.readMterm(id, buffer, buffer.length);
                    if (res > 0) {
                        outputStream.write(buffer,0,res);
                        outputStream.flush();
                        String str = outputStream.toString();
                        Log.d("LIBMTERM", str);
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                out.setText(str);
                            }
                        });
                    }
                    TimeUnit.MILLISECONDS.sleep(1);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        });
        readThread.start();
    }
}