package com.pccontroler.action;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class AuthentificationAction extends PCControlerAction
{
	public String password;
	
	public AuthentificationAction(String password)
	{
		this.password = password;
	}
	
	public static AuthentificationAction parse(DataInputStream dis) throws IOException
	{
		String password = dis.readUTF();
		
		return new AuthentificationAction(password);
	}
	
	public void toDataOutputStream(DataOutputStream dos) throws IOException
	{
		dos.writeByte(AUTHENTIFICATION);
		dos.writeUTF(this.password);
	}
}
