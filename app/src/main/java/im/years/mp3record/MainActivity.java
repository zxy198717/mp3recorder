package im.years.mp3record;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import im.years.mp3recorder.RecMicToMp3;

public class MainActivity extends AppCompatActivity {

    private RecMicToMp3 mRecMicToMp3 = new RecMicToMp3(
            Environment.getExternalStorageDirectory() + "/mezzo.mp3", 8000);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if (ContextCompat.checkSelfPermission(this,
                Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {

            //如果App的权限申请曾经被用户拒绝过，就需要在这里跟用户做出解释
            if (ActivityCompat.shouldShowRequestPermissionRationale(this,
                    Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
                Toast.makeText(this, "please give me the permission", Toast.LENGTH_SHORT).show();
            } else {
                //进行权限请求
                ActivityCompat.requestPermissions(this,
                        new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE},
                        100);
            }
        }

        final TextView statusTextView = (TextView) findViewById(R.id.textView);

        mRecMicToMp3.setHandle(new Handler() {
            @Override
            public void handleMessage(Message msg) {
                switch (msg.what) {
                    case RecMicToMp3.MSG_REC_STARTED:
                        statusTextView.setText("録音中");
                        break;
                    case RecMicToMp3.MSG_REC_STOPPED:
                        statusTextView.setText("");
                        break;
                    case RecMicToMp3.MSG_ERROR_GET_MIN_BUFFERSIZE:
                        statusTextView.setText("");
                        Toast.makeText(MainActivity.this,
                                "録音が開始できませんでした。この端末が録音をサポートしていない可能性があります。",
                                Toast.LENGTH_LONG).show();
                        break;
                    case RecMicToMp3.MSG_ERROR_CREATE_FILE:
                        statusTextView.setText("");
                        Toast.makeText(MainActivity.this, "ファイルが生成できませんでした",
                                Toast.LENGTH_LONG).show();
                        break;
                    case RecMicToMp3.MSG_ERROR_REC_START:
                        statusTextView.setText("");
                        Toast.makeText(MainActivity.this, "録音が開始できませんでした",
                                Toast.LENGTH_LONG).show();
                        break;
                    case RecMicToMp3.MSG_ERROR_AUDIO_RECORD:
                        statusTextView.setText("");
                        Toast.makeText(MainActivity.this, "録音ができませんでした",
                                Toast.LENGTH_LONG).show();
                        break;
                    case RecMicToMp3.MSG_ERROR_AUDIO_ENCODE:
                        statusTextView.setText("");
                        Toast.makeText(MainActivity.this, "エンコードに失敗しました",
                                Toast.LENGTH_LONG).show();
                        break;
                    case RecMicToMp3.MSG_ERROR_WRITE_FILE:
                        statusTextView.setText("");
                        Toast.makeText(MainActivity.this, "ファイルの書き込みに失敗しました",
                                Toast.LENGTH_LONG).show();
                        break;
                    case RecMicToMp3.MSG_ERROR_CLOSE_FILE:
                        statusTextView.setText("");
                        Toast.makeText(MainActivity.this, "ファイルの書き込みに失敗しました",
                                Toast.LENGTH_LONG).show();
                        break;
                    default:
                        break;
                }
            }
        });

        Button startButton = (Button) findViewById(R.id.button);
        startButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mRecMicToMp3.start();
            }
        });
        Button stopButton = (Button) findViewById(R.id.button2);
        stopButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mRecMicToMp3.stop();
            }
        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mRecMicToMp3.stop();
    }
}
