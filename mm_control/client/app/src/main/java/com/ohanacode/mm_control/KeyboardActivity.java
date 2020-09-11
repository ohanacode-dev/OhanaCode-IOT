package com.ohanacode.mm_control;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;

public class KeyboardActivity extends AppCompatActivity implements View.OnClickListener{

    private final String TAG = "KeyboardActivity";
    private DiscoveryAndUdpComms udpSender;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        udpSender = DiscoveryAndUdpComms.getInstance(this);

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
        findViewById(R.id.btn_div).setOnClickListener(this);
        findViewById(R.id.btn_eq).setOnClickListener(this);
        findViewById(R.id.btn_colon).setOnClickListener(this);
        findViewById(R.id.btn_semicolon).setOnClickListener(this);
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
        findViewById(R.id.btn_close).setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        byte codeType = CommandData.CODE_NONE;
        byte codeValue = 0;

        switch (v.getId()) {
            case R.id.btn_0:
                codeType = CommandData.CODE_ASCII;
                codeValue = '0';
                break;
            case R.id.btn_1:
                codeType = CommandData.CODE_ASCII;
                codeValue = '1';
                break;
            case R.id.btn_2:
                codeType = CommandData.CODE_ASCII;
                codeValue = '2';
                break;
            case R.id.btn_3:
                codeType = CommandData.CODE_ASCII;
                codeValue = '3';
                break;
            case R.id.btn_4:
                codeType = CommandData.CODE_ASCII;
                codeValue = '4';
                break;
            case R.id.btn_5:
                codeType = CommandData.CODE_ASCII;
                codeValue = '5';
                break;
            case R.id.btn_6:
                codeType = CommandData.CODE_ASCII;
                codeValue = '6';
                break;
            case R.id.btn_7:
                codeType = CommandData.CODE_ASCII;
                codeValue = '7';
                break;
            case R.id.btn_8:
                codeType = CommandData.CODE_ASCII;
                codeValue = '8';
                break;
            case R.id.btn_9:
                codeType = CommandData.CODE_ASCII;
                codeValue = '9';
                break;

            case R.id.btn_a:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'a';
                break;
            case R.id.btn_b:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'b';
                break;
            case R.id.btn_c:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'c';
                break;
            case R.id.btn_d:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'd';
                break;
            case R.id.btn_e:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'e';
                break;
            case R.id.btn_f:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'f';
                break;
            case R.id.btn_g:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'g';
                break;
            case R.id.btn_h:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'h';
                break;
            case R.id.btn_i:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'i';
                break;
            case R.id.btn_j:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'j';
                break;
            case R.id.btn_k:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'k';
                break;
            case R.id.btn_l:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'l';
                break;
            case R.id.btn_m:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'm';
                break;
            case R.id.btn_n:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'n';
                break;
            case R.id.btn_o:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'o';
                break;
            case R.id.btn_p:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'p';
                break;
            case R.id.btn_q:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'q';
                break;
            case R.id.btn_r:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'r';
                break;
            case R.id.btn_s:
                codeType = CommandData.CODE_ASCII;
                codeValue = 's';
                break;
            case R.id.btn_t:
                codeType = CommandData.CODE_ASCII;
                codeValue = 't';
                break;
            case R.id.btn_u:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'u';
                break;
            case R.id.btn_v:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'v';
                break;
            case R.id.btn_w:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'w';
                break;
            case R.id.btn_x:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'x';
                break;
            case R.id.btn_y:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'y';
                break;
            case R.id.btn_z:
                codeType = CommandData.CODE_ASCII;
                codeValue = 'z';
                break;


            case R.id.btn_plus:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_PLUS;
                break;
            case R.id.btn_minus:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_MINUS;
                break;
            case R.id.btn_div:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_SLASH;
                break;
            case R.id.btn_eq:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_EQUAL;
                break;
            case R.id.btn_colon:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_COLON;
                break;
            case R.id.btn_semicolon:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_SEMICOLON;
                break;
            case R.id.btn_percent:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_PERCENT;
                break;
            case R.id.btn_lessthan:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_LESS;
                break;
            case R.id.btn_morethan:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_GREATER;
                break;
            case R.id.btn_question:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_QUESTION;
                break;
            case R.id.btn_quote:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_QUOTE;
                break;
            case R.id.btn_tild:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_TILDE;
                break;
            case R.id.btn_underscore:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_UNDERSCORE;
                break;
            case R.id.btn_exclamation:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_EXCLAMATION;
                break;
            case R.id.btn_caps:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_CAPS;
                break;
            case R.id.btn_at:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_MONKEY;
                break;
            case R.id.btn_comma:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_COMMA;
                break;
            case R.id.btn_dot:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_PERIOD;
                break;
            case R.id.btn_vol_dn:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_VOLDN;
                break;
            case R.id.btn_vol_up:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_VOLUP;
                break;
            case R.id.btn_play:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_PLAY;
                break;
            case R.id.btn_page_down:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_PGDN;
                break;
            case R.id.btn_page_up:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_PGUP;
                break;
            case R.id.btn_arrow_left:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_LEFT;
                break;
            case R.id.btn_arrow_right:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_RIGHT;
                break;
            case R.id.btn_arrow_up:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_UP;
                break;
            case R.id.btn_arrow_down:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_DOWN;
                break;
            case R.id.btn_space:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_SPACE;
                break;
            case R.id.btn_enter:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_ENTER;
                break;
            case R.id.btn_backspace:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_BACKSPACE;
                break;
            case R.id.btn_close:
                codeType = CommandData.CODE_SPECIAL;
                codeValue = CommandData.KEY_CLOSE;
                break;

            default:
                break;
        }

        if(codeType != CommandData.CODE_NONE){
            byte[] msg = new byte[4];
            msg[0] = udpSender.getNextPacketNumber();
            msg[1] = codeType;
            msg[2] = codeValue;
            msg[3] = 0;

            udpSender.sendUdpMsg(msg);
        }
    }
}
