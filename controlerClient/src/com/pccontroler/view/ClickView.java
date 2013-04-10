package com.pccontroler.view;

import android.content.Context;
import android.content.SharedPreferences;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.widget.Button;

import com.pccontroler.R;
import com.pccontroler.action.MouseClickAction;
import com.pccontroler.activity.ControlActivity;
import com.pccontroler.app.PCControler;

public class ClickView extends Button
{
	private static final float MOUSE_WHEEL_SENSITIVITY_FACTOR = 10;
	
	private ControlActivity controlActivity;
	private PCControler application;
	private SharedPreferences preferences;
	
	private byte button;
	private boolean hold;
	private long holdDelay;
	private boolean touchMoveAble;
	private boolean wheelMove;
	
	private float moveSensitivity;
	private float wheelSensitivity;
	private float wheelAcceleration;
	private float wheelPrevious;
	private float wheelResult;
	
	public ClickView(Context context, AttributeSet attrs)
	{
		super(context, attrs);
		
		this.controlActivity = (ControlActivity) context;
		this.application = (PCControler) this.controlActivity.getApplication();
		this.touchMoveAble = false;
		this.preferences = application.getPreferences();
		
		switch (this.getId())
		{
			case R.id.leftClickView:
				this.button = MouseClickAction.BUTTON_LEFT;
				break;
			case R.id.middleClickView:
				this.button = MouseClickAction.BUTTON_MIDDLE;
				this.touchMoveAble = true;
				break;
			case R.id.rightClickView:
				this.button = MouseClickAction.BUTTON_RIGHT;
				break;
			default:
				this.button = MouseClickAction.BUTTON_NONE;
				break;
		}
		
		this.hold = false;
		
		this.holdDelay = Long.parseLong(this.application.getPreferences().getString("control_hold_delay", null));
		
		float screenDensity = this.getResources().getDisplayMetrics().density;
		
		this.moveSensitivity = Float.parseFloat(this.preferences.getString("control_sensitivity", null));
		this.moveSensitivity /= screenDensity;
		
		this.wheelSensitivity = this.moveSensitivity / MOUSE_WHEEL_SENSITIVITY_FACTOR;
		this.wheelAcceleration = Float.parseFloat(this.preferences.getString("control_acceleration", null));
	}
	
	public boolean isHold()
	{
		return hold;
	}
	
	public void setHold(boolean hold)
	{
		this.hold = hold;
	}
	
	public boolean onTouchEvent(MotionEvent event)
	{
		switch (event.getAction())
		{
			case MotionEvent.ACTION_MOVE:
			{
				this.onTouchMove(event);
				break;
			}
			
			case MotionEvent.ACTION_DOWN:
			{
				this.onTouchDown(event);
				break;
			}
			
			case MotionEvent.ACTION_UP:
			{
				this.onTouchUp(event);
				break;
			}
			
			default:
				break;
		}
		
		return true;
	}
	
	private void onTouchDown(MotionEvent event)
	{
		if (!this.hold)
		{
			// Toast.makeText(this.application, "down",
			// Toast.LENGTH_SHORT).show();
			
			if (this.touchMoveAble)
				
				this.onTouchDownMouseWheel(event);
			
			else
				
				this.controlActivity.mouseClick(this.button, MouseClickAction.STATE_DOWN);
			
			this.setPressed(true);
			
			// this.application.vibrate(50);
			
		}
		else
		{
			this.hold = false;
		}
	}
	
	private void onTouchDownMouseWheel(MotionEvent event)
	{
		this.wheelPrevious = event.getRawY();
		this.wheelResult = 0;
	}
	
	private void onTouchMoveMouseWheel(MotionEvent event)
	{
		float wheelRaw = event.getRawY() - this.wheelPrevious;
		wheelRaw *= this.wheelSensitivity;
		wheelRaw = (float) ((Math.pow(Math.abs(wheelRaw), this.wheelAcceleration) * Math.signum(wheelRaw)));
		wheelRaw += this.wheelResult;
		int wheelFinal = Math.round(wheelRaw);
		
		if (wheelFinal != 0)
		{
			// Log.i("fuck", wheelFinal + "");
			// Toast.makeText(this.application, wheelFinal + "",
			// Toast.LENGTH_SHORT).show();
			this.controlActivity.mouseWheel(-wheelFinal);
			this.wheelMove = true;
		}
		
		this.wheelResult = wheelRaw - wheelFinal;
		this.wheelPrevious = event.getRawY();
	}
	
	private void onTouchMove(MotionEvent event)
	{
		if (this.touchMoveAble)
		{
			this.onTouchMoveMouseWheel(event);
			return;
		}
		
		// if (this.wheelMove)
		//
		// return;
		
		if (!this.hold && event.getEventTime() - event.getDownTime() >= this.holdDelay)
		{
			this.hold = true;
			
			// this.application.vibrate(100);
		}
	}
	
	private void onTouchUp(MotionEvent event)
	{
		if (this.wheelMove)
		{
			this.wheelMove = false;
			
			this.setPressed(false);
			
			// this.controlActivity.mouseClick(this.button,
			// MouseClickAction.STATE_UP);
			
			return;
		}
		
		if (!this.hold)
		{
			if (this.touchMoveAble)
				
				this.controlActivity.mouseClick(this.button, MouseClickAction.STATE_DOWN);
			
			this.controlActivity.mouseClick(this.button, MouseClickAction.STATE_UP);
			
			this.setPressed(false);
		}
	}
}