package com.example.simple;

import androidx.appcompat.app.AppCompatActivity;
import android.view.*;
import android.os.Bundle;
import android.widget.Button;
import android.widget.TextView;
import java.util.ArrayList;


public class MainActivity extends AppCompatActivity {
    private int opnd = 0;
    private int temp = 0;
    private int result = 0;
    private ArrayList<Integer> number = new ArrayList<Integer>();
    private ArrayList<Character> op = new ArrayList<Character>();
    private ArrayList<Character> op1 = new ArrayList<Character>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        number.add(0);
        op.add('0');
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ((Button)findViewById(R.id.button_0)).setOnClickListener(
                (v) -> { appendDigit(0);}
        );

        ((Button)findViewById(R.id.button_1)).setOnClickListener(
                (v) -> { appendDigit(1);}
        );
        ((Button)findViewById(R.id.button_2)).setOnClickListener(
                (v)->{appendDigit(2);}
        );
        ((Button)findViewById(R.id.button_3)).setOnClickListener(
                (v)->{appendDigit(3);}
        );
        ((Button)findViewById(R.id.button_4)).setOnClickListener(
                (v)->{appendDigit(4);}
        );
        ((Button)findViewById(R.id.button_5)).setOnClickListener(
                (v)->{appendDigit(5);}
        );
        ((Button)findViewById(R.id.button_6)).setOnClickListener(
                (v)->{appendDigit(6);}
        );
        ((Button)findViewById(R.id.button_7)).setOnClickListener(
                (v)->{appendDigit(7);}
        );
        ((Button)findViewById(R.id.button_8)).setOnClickListener(
                (v)->{appendDigit(8);}
        );
        ((Button)findViewById(R.id.button_9)).setOnClickListener(
                (v)->{appendDigit(9);}
        );

        ((Button)findViewById(R.id.button_c)).setOnClickListener(
                (v)-> {
                    op.clear();
                    number.clear();
                    temp=0;
                    opnd = 0;
                    result=0;
                    final TextView txtResult = (TextView) findViewById(R.id.text_result);
                    txtResult.setText("");
                }
        );
        ((Button)findViewById(R.id.button_add)).setOnClickListener(
                (v)-> {
                    number.add(opnd);
                    op.add('+');
                    opnd = 0;
                    final TextView txtResult = (TextView) findViewById(R.id.text_result);
                    txtResult.setText("");

                }
        );
        ((Button)findViewById(R.id.button_sub)).setOnClickListener(
                (v)-> {
                    number.add(opnd);
                    op.add('-');
                    opnd = 0;
                    final TextView txtResult = (TextView) findViewById(R.id.text_result);
                    txtResult.setText("");

                }
        );
        ((Button)findViewById(R.id.button_mul)).setOnClickListener(
                (v)-> {
                    number.add(opnd);
                    op.add('*');
                    opnd = 0;
                    final TextView txtResult = (TextView) findViewById(R.id.text_result);
                    txtResult.setText("");

                }
        );
        ((Button)findViewById(R.id.button32_div)).setOnClickListener(
                (v)-> {
                    number.add(opnd);
                    op.add('/');
                    opnd = 0;
                    final TextView txtResult = (TextView) findViewById(R.id.text_result);
                    txtResult.setText("");

                }
        );
        ((Button)findViewById(R.id.button_calc)).setOnClickListener(
                (v)-> {
                    int i=1;
                    op.add('=');
                    number.add(opnd);
                    while(op.get(i)!='=') {
                        if (op.get(i) == '*') {
                            temp = number.get(i) * number.get(i + 1);
                            number.remove(i);
                            number.remove(i);
                            number.add(i, temp);
                            op.remove(i);
                        } else if (op.get(i) == '/') {
                            temp = number.get(i) / number.get(i + 1);
                            number.remove(i);
                            number.remove(i);
                            number.add(i, temp);
                            op.remove(i);
                        } else if (op.get(i) == '+') {
                            i++;
                        } else if (op.get(i) == '-') {
                            i++;
                        }
                    }


                    result=number.get(1);
                    for(i=1;i<=number.size()-1;i++){
                        if(op.get(i)=='+'){
                            result=result+number.get(i+1);
                        }
                        else if(op.get(i)=='-'){
                            result=result-number.get(i+1);
                        }
                    }

                    final TextView txtResult = (TextView) findViewById(R.id.text_result);
                    txtResult.setText(""+result);

                    temp=0;
                    opnd = 0;
                    result=0;
                    op.clear();
                    number.clear();
                }
        );


    }

    private void appendDigit(int a){
        opnd=opnd*10 +a;
        final TextView txtResult = (TextView)findViewById(R.id.text_result);
        txtResult.setText(""+opnd);
    }
}