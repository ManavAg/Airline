// JavaScript Document
var LangCode;

$(document).ready(function() 
{
	//$('.classysocial').ClassySocial();
	LangCode=1; //1 for EN, 2 for FR
	LoadApps();
	ResizeTabScreen();
	$('CVTable tr').click(function(){
        window.location = $(this).attr('href');
        return false;
    });
});
$(window).resize(function() {ResizeTabScreen();});
function ResizeTabScreen()
{
		var AR;
		$("#tablet").css("height", $(window).height()+"px");
		AR = $("#tablet").height()/$("#tablet").width();
		if(AR>0.9 || AR<0.7)
		{	
			$("#tablet").height(0.8*$("#tablet").width());
		}
		
		$("#screen").css("margin-top", 0.13*$("#tablet").height()+"px");
		$("#screen").css("height", 0.73*$("#tablet").height()+"px");
		$("#Contacts").css("margin-top", 0.12*$("#tablet").height()+"px");
		$("#Signature").css("margin-top", 0.04*$("#tablet").height()+"px");
		$("#Signature").css("height", 0.08*$("#tablet").height()+"px");
		$(".app").css("height", 0.20*$("#tablet").height()+"px");
		$("#apps1").css("margin-top", 0.20*$("#tablet").height()+"px");
		$("#apps2").css("margin-top", 0.04*$("#tablet").height()+"px");
		$("#Lang1").css("margin-top", 0.05*$("#tablet").height()+"px");
		$("#Lang2").css("margin-top", 0.08*$("#tablet").height()+"px");
		$("#Lang1").css("font-size", 0.03*$("#tablet").height()+"px");
		$("#Lang2").css("font-size", 0.03*$("#tablet").height()+"px");
		
}

   function LoadApps()
   {
	   var text1="";
	   var text2="";
		for (i=1;i<5;i++)
		{
			switch (LangCode)
			{
				case 1: 
					{
						text1=text1+"<img class='app' src='Pics/" + i + ".jpg' height= '25%' width= '20%' style='margin-left:3%' OnClick=ContentEN("+i+")  data-toggle='modal' data-target='#myModal'/>"
					text2=text2+"<img class='app' src='Pics/" + (i+4) + ".jpg' height= '25%' width= '20%' style='margin-left:3%' OnClick=ContentEN("+(i+4)+")  data-toggle='modal' data-target='#myModal' />"
					break;
					}
				case 2:
					{
						text1=text1+"<img class='app' src='Pics/" + i + "f.jpg' height= '25%' width= '20%' style='margin-left:3%' OnClick=ContentFR("+i+")  data-toggle='modal' data-target='#myModal'/>"
					text2=text2+"<img class='app' src='Pics/" + (i+4) + "f.jpg' height= '25%' width= '20%' style='margin-left:3%' OnClick=ContentFR("+(i+4)+")  data-toggle='modal' data-target='#myModal' />"
					}
			}
		}
	   $("#apps1").html(text1);
	   $("#apps1").css("margin-top","130px");
	   $("#apps1").css("margin-left","5%");
   	   $("#apps2").html(text2);
	   $("#apps2").css("margin-top","20px");
	   $("#apps2").css("margin-left","5%");

   }
	function DownNameTab()
	{
		if($("#Contacts").css("height")=="0px")
			{
				$("#Contacts").css("height",0.04*$("#tablet").height()+"px");
				$(".contact").css("height",0.04*$("#tablet").height()+"px");
			}
		else 
			{
				$("#Contacts").css("height","0px");
				$(".contact").css("height","0px");
			}
	}
	function DownLangTab()
	{
		switch(LangCode)
			{
				case 1:
					{
						if($("#Lang2").html()=="")
							{$("#Lang2").html("FR");$("#Lang1").html("EN");}
						else $("#Lang2").html("");
						break;
					}
				case 2:
					{
						if($("#Lang2").html()=="")
							{$("#Lang2").html("EN");$("#Lang1").html("FR");}
						else $("#Lang2").html("");
					}
			}
				
	}
   	function SelectLang()
	{
		if ($("#Lang2").html()=="EN")
		LoadLanguage(1);
		if ($("#Lang2").html()=="FR")
		LoadLanguage(2);
	}
	function LoadLanguage(a)
	{
		if (a==1)
		{
			$("#Lang1").html("EN &#9660;");
			$("#Lang2").html("");
			LangCode=1;
			LoadApps();
		}
		else
		{
			$("#Lang1").html("FR &#9660;");
			$("#Lang2").html("");
			LangCode=2;
			LoadApps();
		}
	}
	
	$.fn.stars = function() {
    return $(this).each(function() {
        $(this).html($('<span />').width(Math.max(0, (Math.min(5, parseFloat($(this).html())))) * 16));
    });
}	
   function Bookmarking(b)
   {
	   	for (i=1;i<8;i++)
		{
			$("#Bookmark"+i).css("margin-top","0px");
   		}
		   $("#Bookmark"+b).css("margin-top","45px");
   }