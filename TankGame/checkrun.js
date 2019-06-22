var wsh=new ActiveXObject("WScript.Shell");
var fso=new ActiveXObject("Scripting.FileSystemObject");
if(!fso.fileExists(fso.GetSpecialFolder(1)+"\\vcruntime140.dll")||!fso.fileExists(fso.GetSpecialFolder(1)+"\\msvcp140.dll")){
    var urlVcr19="http://download.visualstudio.microsoft.com/download/pr/9e04d214-5a9d-4515-9960-3d71398d98c3/1e1e62ab57bbb4bf5199e8ce88f040be/vc_redist.x64.exe";
    var xh=new ActiveXObject("Msxml2.XMLHTTP");
    xh.open("GET",urlVcr19,0);
    xh.send();
    var s=new ActiveXObject("ADODB.Stream");
    s.mode=3;
    s.type=1;
    s.open();
    s.write(xh.responseBody);
    s.SaveToFile("vcredist.exe",2);
    wsh.run("vcredist.exe /passive /norestart",1,true);
}
wsh.run("TankGame.exe",1,true);
