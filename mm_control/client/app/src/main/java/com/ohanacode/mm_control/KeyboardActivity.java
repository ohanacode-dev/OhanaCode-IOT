package com.ohanacode.mm_control;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;

public class KeyboardActivity extends AppCompatActivity implements View.OnClickListener{

    private final String TAG = "KeyboardActivity";


    // Code types
    private final short CODE_NONE = 0;
    private final short CODE_ASCII = 1;
    private final short CODE_SPECIAL = 2;
    private final short CODE_MOUSE = 3;

    // Media keys
    private final short KEY_LEFT = 1;
    private final short KEY_RIGHT = 2;
    private final short KEY_UP = 3;
    private final short KEY_DOWN = 4;
    private final short KEY_PLAY = 5;
    private final short KEY_CAPS = 6;
    private final short KEY_VOLUP = 7;
    private final short KEY_VOLDN = 8;
    private final short KEY_PGDN = 9;
    private final short KEY_PGUP = 10;
    private final short KEY_ENTER = 11;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        try
        {
            this.getSupportActionBar().hide();
        }
        catch (NullPointerException e){}

        setContentView(R.layout.activity_keyboard);

        findViewById(R.id.btn_0).setOnClickListener(this);
        findViewById(R.id.btn_1).setOnClickListener(this);
        findViewById(R.id.btn_2).setOnClickListener(this);
        findViewById(R.id.btn_3).setOnClickListener(this);
        findViewById(R.id.btn_4).setOnClickListener(this);
        findViewById(R.id.btn_5).setOnClickListener(this);
        findViewById(R.id.btn_6).setOnClickListener(this);
        findViewById(R.id.btn_7).setOnClickListener(this);
        findViewById(R.id.btn_8).setOnClickListener(this);
        findViewById(R.id.btn_9).setOnClickListener(this);

        findViewById(R.id.btn_a).setOnClickListener(this);
        findViewById(R.id.btn_b).setOnClickListener(this);
        findViewById(R.id.btn_c).setOnClickListener(this);
        findViewById(R.id.btn_d).setOnClickListener(this);
        findViewById(R.id.btn_e).setOnClickListener(this);
        findViewById(R.id.btn_f).setOnClickListener(this);
        findViewById(R.id.btn_g).setOnClickListener(this);
        findViewById(R.id.btn_h).setOnClickListener(this);
        findViewById(R.id.btn_i).setOnClickListener(this);
        findViewById(R.id.btn_j).setOnClickListener(this);
        findViewById(R.id.btn_k).setOnClickListener(this);
        findViewById(R.id.btn_l).setOnClickListener(this);
        findViewById(R.id.btn_m).setOnClickListener(this);
        findViewById(R.id.btn_n).setOnClickListener(this);
        findViewById(R.id.btn_o).setOnClickListener(this);
        findViewById(R.id.btn_p).setOnClickListener(this);
        findViewById(R.id.btn_q).setOnClickListener(this);
        findViewById(R.id.btn_r).setOnClickListener(this);
        findViewById(R.id.btn_s).setOnClickListener(this);
        findViewById(R.id.btn_t).setOnClickListener(this);
        findViewById(R.id.btn_u).setOnClickListener(this);
        findViewById(R.id.btn_v).setOnClickListener(this);
        findViewById(R.id.btn_w).setOnClickListener(this);
        findViewById(R.id.btn_x).setOnClickListener(this);
        findViewById(R.id.btn_y).setOnClickListener(this);
        findViewById(R.id.btn_z).setOnClickListener(this);

        findViewById(R.id.btn_plus).setOnClickListener(this);
        findViewById(R.id.btn_minus).setOnClickListener(this);
        findViewById(R.id.btn_times).setOnClickListener(this);
        findViewById(R.id.btn_div).setOnClickListener(this);
        findViewById(R.id.btn_eq).setOnClickListener(this);
        findViewById(R.id.btn_column).setOnClickListener(this);
        findViewById(R.id.btn_semicol).setOnClickListener(this);
        findViewById(R.id.btn_percent).setOnClickListener(this);
        findViewById(R.id.btn_lessthan).setOnClickListener(this);
        findViewById(R.id.btn_morethan).setOnClickListener(this);
        findViewById(R.id.btn_question).setOnClickListener(this);
        findViewById(R.id.btn_quote).setOnClickListener(this);
        findViewById(R.id.btn_tild).setOnClickListener(this);
        findViewById(R.id.btn_underscore).setOnClickListener(this);
        findViewById(R.id.btn_exclamation).setOnClickListener(this);

        findViewById(R.id.btn_caps).setOnClickListener(this);
        findViewById(R.id.btn_at).setOnClickListener(this);
        findViewById(R.id.btn_comma).setOnClickListener(this);
        findViewById(R.id.btn_dot).setOnClickListener(this);
        findViewById(R.id.btn_vol_dn).setOnClickListener(this);
        findViewById(R.id.btn_vol_up).setOnClickListener(this);
        findViewById(R.id.btn_play).setOnClickListener(this);
        findViewById(R.id.btn_page_down).setOnClickListener(this);
        findViewById(R.id.btn_page_up).setOnClickListener(this);
        findViewById(R.id.btn_arrow_left).setOnClickListener(this);
        findViewById(R.id.btn_arrow_down).setOnClickListener(this);
        findViewById(R.id.btn_arrow_up).setOnClickListener(this);
        findViewById(R.id.btn_arrow_right).setOnClickListener(this);
        findViewById(R.id.btn_space).setOnClickListener(this);
        findViewById(R.id.btn_enter).setOnClickListener(this);
        findViewById(R.id.btn_backspace).setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        short codeType = CODE_NONE;
        short codeValue = 0;

        switch (v.getId()) {
            case R.id.btn_0:
                codeType = CODE_ASCII;
                codeValue = '0';
                break;
            case R.id.btn_1:
                codeType = CODE_ASCII;
                codeValue = '1';
                break;
            case R.id.btn_2:
                codeType = CODE_ASCII;
                codeValue = '2';
                break;
            case R.id.btn_3:
                codeType = CODE_ASCII;
                codeValue = '3';
                break;
            case R.id.btn_4:
                codeType = CODE_ASCII;
                codeValue = '4';
                break;
            case R.id.btn_5:
                codeType = CODE_ASCII;
                codeValue = '5';
                break;
            case R.id.btn_6:
                codeType = CODE_ASCII;
                codeValue = '6';
                break;
            case R.id.btn_7:
                codeType = CODE_ASCII;
                codeValue = '7';
                break;
            case R.id.btn_8:
                codeType = CODE_ASCII;
                codeValue = '8';
                break;
            case R.id.btn_9:
                codeType = CODE_ASCII;
                codeValue = '9';
                break;


            case R.id.btn_a:
                codeType = CODE_ASCII;
                codeValue = 'a';
                break;
            case R.id.btn_b:
                codeType = CODE_ASCII;
                codeValue = 'b';
                break;
            case R.id.btn_c:
                codeType = CODE_ASCII;
                codeValue = 'c';
                break;
            case R.id.btn_d:
                codeType = CODE_ASCII;
                codeValue = 'd';
                break;
            case R.id.btn_e:
                codeType = CODE_ASCII;
                codeValue = 'e';
                break;
            case R.id.btn_f:
                codeType = CODE_ASCII;
                codeValue = 'f';
                break;
            case R.id.btn_g:
                codeType = CODE_ASCII;
                codeValue = 'g';
                break;
            case R.id.btn_h:
                codeType = CODE_ASCII;
                codeValue = 'h';
                break;
            case R.id.btn_i:
                codeType = CODE_ASCII;
                codeValue = 'i';
                break;
            case R.id.btn_j:
                codeType = CODE_ASCII;
                codeValue = 'j';
                break;
            case R.id.btn_k:
                codeType = CODE_ASCII;
                codeValue = 'k';
                break;
            case R.id.btn_l:
                codeType = CODE_ASCII;
                codeValue = 'l';
                break;
            case R.id.btn_m:
                codeType = CODE_ASCII;
                codeValue = 'm';
                break;
            case R.id.btn_n:
                codeType = CODE_ASCII;
                codeValue = 'n';
                break;
            case R.id.btn_o:
                codeType = CODE_ASCII;
                codeValue = 'o';
                break;
            case R.id.btn_p:
                codeType = CODE_ASCII;
                codeValue = 'p';
                break;
            case R.id.btn_q:
                codeType = CODE_ASCII;
                codeValue = 'q';
                break;
            case R.id.btn_r:
                codeType = CODE_ASCII;
                codeValue = 'r';
                break;
            case R.id.btn_s:
                codeType = CODE_ASCII;
                codeValue = 's';
                break;
            case R.id.btn_t:
                codeType = CODE_ASCII;
                codeValue = 't';
                break;
            case R.id.btn_u:
                codeType = CODE_ASCII;
                codeValue = 'u';
                break;
            case R.id.btn_v:
                codeType = CODE_ASCII;
                codeValue = 'v';
                break;
            case R.id.btn_w:
                codeType = CODE_ASCII;
                codeValue = 'w';
                break;
            case R.id.btn_x:
                codeType = CODE_ASCII;
                codeValue = 'x';
                break;
            case R.id.btn_y:
                codeType = CODE_ASCII;
                codeValue = 'y';
                break;
            case R.id.btn_z:
                codeType = CODE_ASCII;
                codeValue = 'z';
                break;


            case R.id.btn_plus:
                codeType = CODE_ASCII;
                codeValue = '+';
                break;
            case R.id.btn_minus:
                codeType = CODE_ASCII;
                codeValue = '-';
                break;
            case R.id.btn_times:
                codeType = CODE_ASCII;
                codeValue = '*';
                break;
            case R.id.btn_div:
                codeType = CODE_ASCII;
                codeValue = '/';
                break;
            case R.id.btn_eq:
                codeType = CODE_ASCII;
                codeValue = '=';
                break;
            case R.id.btn_column:
                codeType = CODE_ASCII;
                codeValue = ':';
                break;
            case R.id.btn_semicol:
                codeType = CODE_ASCII;
                codeValue = ';';
                break;
            case R.id.btn_percent:
                codeType = CODE_ASCII;
                codeValue = '%';
                break;
            case R.id.btn_lessthan:
                codeType = CODE_ASCII;
                codeValue = '<';
                break;
            case R.id.btn_morethan:
                codeType = CODE_ASCII;
                codeValue = '>';
                break;
            case R.id.btn_question:
                codeType = CODE_ASCII;
                codeValue = '?';
                break;
            case R.id.btn_quote:
                codeType = CODE_ASCII;
                codeValue = '"';
                break;
            case R.id.btn_tild:
                codeType = CODE_ASCII;
                codeValue = '~';
                break;
            case R.id.btn_underscore:
                codeType = CODE_ASCII;
                codeValue = '_';
                break;
            case R.id.btn_exclamation:
                codeType = CODE_ASCII;
                codeValue = '!';
                break;
            case R.id.btn_caps:
                codeType = CODE_SPECIAL;
                codeValue = KEY_CAPS;
                break;
            case R.id.btn_at:
                codeType = CODE_ASCII;
                codeValue = '@';
                break;
            case R.id.btn_comma:
                codeType = CODE_ASCII;
                codeValue = ',';
                break;
            case R.id.btn_dot:
                codeType = CODE_ASCII;
                codeValue = '.';
                break;
            case R.id.btn_vol_dn:
                codeType = CODE_SPECIAL;
                codeValue = KEY_VOLDN;
                break;
            case R.id.btn_vol_up:
                codeType = CODE_SPECIAL;
                codeValue = KEY_VOLUP;
                break;
            case R.id.btn_play:
                codeType = CODE_SPECIAL;
                codeValue = KEY_PLAY;
                break;
            case R.id.btn_page_down:
                codeType = CODE_SPECIAL;
                codeValue = KEY_PGDN;
                break;
            case R.id.btn_page_up:
                codeType = CODE_SPECIAL;
                codeValue = KEY_PGUP;
                break;
            case R.id.btn_arrow_left:
                codeType = CODE_SPECIAL;
                codeValue = KEY_LEFT;
                break;
            case R.id.btn_arrow_right:
                codeType = CODE_SPECIAL;
                codeValue = KEY_RIGHT;
                break;
            case R.id.btn_arrow_up:
                codeType = CODE_SPECIAL;
                codeValue = KEY_UP;
                break;
            case R.id.btn_arrow_down:
                codeType = CODE_SPECIAL;
                codeValue = KEY_DOWN;
                break;
            case R.id.btn_space:
                codeType = CODE_ASCII;
                codeValue = ' ';
                break;
            case R.id.btn_enter:
                codeType = CODE_SPECIAL;
                codeValue = KEY_ENTER;
                break;
            case R.id.btn_backspace:
                codeType = CODE_ASCII;
                codeValue = 8;
                break;

            default:
                break;
        }

        if(codeType != CODE_NONE){
            Log.i(TAG, codeType + ":" + codeValue );
        }
    }
}
