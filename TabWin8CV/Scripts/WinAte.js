// JavaScript Document
var LangCode;

$(document).ready(function() 
{
	var insideTags="";
	insideTags='<div class="screen"><div class="Signature" onClick="DownNameTab();"><div class="SignatureText"><div class="SFirstName">'+ $(".tablet").attr("SignatureFirstName")+ '</div><div class="SLastName">'+ $(".tablet").attr("SignatureLastName")+ '</div><div class="SEmail">'+ $(".tablet").attr("SignatureEmail")+ '</div></div><img class="SignImg" src="'+ $(".tablet").attr("SignatureImg")+ '"/></div><div class="Contacts">'+ ReturnContacts() +'</div><div class="apps1"></div>      <div class="apps2"></div><div class="modal fade w8modal" id="myModal" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true"><div class="modal-dialog w8modal-dialog"><div class="modal-content w8modal-content"><div class="modal-header w8modal-header"><h4 class="modal-title w8modal-title" id="myModalLabel" style="display:inline-block; width:50%">Modal title</h4><button type="button " class="btn btn-danger btn-xs" data-dismiss="modal" aria-hidden="true" style="margin-left:45%; width:3%; padding-left:0.5%; display:inline-block; position:fixed">&times;</button></div><div class="modal-body w8modal-body"></div></div></div></div></div>'
	$(".tablet").html(insideTags);
	if ($(".tablet").attr('wallpaper'))
		$(".screen").css("background","url(" + $(".tablet").attr('wallpaper') + ")");

	LoadApps();
	ResizeTabScreen();
	});
function LoadApps()
   {
	   var text1="";
	   var text2="";
	   var Napps=0;
	   Napps=parseInt($(".tablet").attr("Napps"));
	   if (Napps<5)
	   {
			for (i=1;i<(Napps+1);i++)
			{text1=text1+"<img class='app' src='" + $('#Wapp'+i).attr('imgsrc') + "' height= '25%' width= '20%' style='margin-left:3%' OnClick=Content("+i+")  data-toggle='modal' data-target='#myModal'/>"}
	   		$(".apps1").html(text1);
	   		$(".apps1").css("margin-top","130px");
	   		$(".apps1").css("margin-left","5%");
	   		$(".apps2").html("");
	   }
	   if (Napps<9 && Napps>4)
	   {
			for (i=1;i<5;i++)
				{text1=text1+"<img class='app' src='" + $('#Wapp'+i).attr('imgsrc') + "' height= '25%' width= '20%' style='margin-left:3%' OnClick=Content("+i+")  data-toggle='modal' data-target='#myModal'/>"}
			for (i=5;i<Napps+1;i++)
				{text2=text2+"<img class='app' src='" + $('#Wapp'+i).attr('imgsrc') + "' height= '25%' width= '20%' style='margin-left:3%' OnClick=Content("+i+")  data-toggle='modal' data-target='#myModal'/>"}
		   	$(".apps1").html(text1);
	   		$(".apps1").css("margin-top","130px");
	   		$(".apps1").css("margin-left","5%");
   	   		$(".apps2").html(text2);
	   		$(".apps2").css("margin-top","20px");
	   		$(".apps2").css("margin-left","5%");
	   }
   }

$(window).resize(function() {ResizeTabScreen();});
function ResizeTabScreen()
{
		var AR;
		$(".tablet").css("height", $(window).height()+"px");
		AR = $(".tablet").height()/$(".tablet").width();
		if(AR>0.9 || AR<0.7)
		{	
			$(".tablet").height(0.8*$(".tablet").width());
		}
		
		$(".screen").css("margin-top", 0.13*$(".tablet").height()+"px");
		$(".screen").css("height", 0.73*$(".tablet").height()+"px");
		$(".screen").css("margin-left", 0.08*$(".tablet").width()+"px");
		$(".screen").css("width", 0.84*$(".tablet").width()+"px");
		$(".Contacts").css("margin-top", 0.15*$(".tablet").height()+"px");
		$(".Signature").css("margin-top", 0.04*$(".tablet").height()+"px");
		$(".Signature").css("height", 0.11*$(".tablet").height()+"px");
		$(".app").css("height", 0.20*$(".tablet").height()+"px");
		$(".apps1").css("margin-top", 0.20*$(".tablet").height()+"px");
		$(".apps2").css("margin-top", 0.04*$(".tablet").height()+"px");
		$(".SFirstName").css("font-size", 0.03*$(".tablet").height()+"px");
		$(".SLastName").css("font-size", 0.015*$(".tablet").height()+"px");
		$(".SEmail").css("font-size", 0.015*$(".tablet").height()+"px");

}

function DownNameTab()
	{
		if($(".Contacts").css("height")=="0px")
			{
				$(".Contacts").css("height",0.04*$(".tablet").height()+"px");
				$(".contact").css("height",0.04*$(".tablet").height()+"px");
			}
		else 
			{
				$(".Contacts").css("height","0px");
				$(".contact").css("height","0px");
			}
	}
function Content(i)
{
	$(".w8modal-title").html($("#Wapp"+i).attr("Header"));
	$(".w8modal-body").html($("#Wapp"+i).html());
}
function ReturnContacts()
{
	var texthtml="";
	NContacts=parseInt($("#WContacts").attr("NContacts"));
	if (NContacts>7)
		NContacts=7;
	for (i=1;i<NContacts+1;i++)
	{
		if ($("#WContacts").attr("Contact"+i)=="email")
			texthtml=texthtml + '<a href="mailto:'+$(".WContacts").attr("ID"+i)+'"><img class="contact" src="Pics/Contacts/email.png"></a>'
		if ($("#WContacts").attr("Contact"+i)=="facebook")
			texthtml=texthtml + '<a href="http://www.facebook.com/'+$(".WContacts").attr("ID"+i)+'"><img class="contact" src="Pics/Contacts/facebook.png"></a>'
		if ($("#WContacts").attr("Contact"+i)=="twitter")
			texthtml=texthtml + '<a href="http://www.twitter.com/'+$(".WContacts").attr("ID"+i)+'"><img class="contact" src="Pics/Contacts/twitter.png"></a>'
		if ($("#WContacts").attr("Contact"+i)=="github")
			texthtml=texthtml + '<a href="http://www.github.com/'+$(".WContacts").attr("ID"+i)+'"><img class="contact" src="Pics/Contacts/github.png"></a>';
		if ($("#WContacts").attr("Contact"+i)=="linkedin")
			texthtml=texthtml + '<a href="http://www.linkedin.com/'+$(".WContacts").attr("ID"+i)+'"><img class="contact" src="Pics/Contacts/linkedin.png"></a>';
		if ($("#WContacts").attr("Contact"+i)=="youtube")
			texthtml=texthtml + '<a href="http://www.youtube.com/'+$(".WContacts").attr("ID"+i)+'"><img class="contact" src="Pics/Contacts/youtube.png"></a>';
		if ($("#WContacts").attr("Contact"+i)=="skype")
			texthtml=texthtml + '<a href="http://www.skype.com/'+$(".WContacts").attr("ID"+i)+'"><img class="contact" src="Pics/Contacts/skype.png"></a>';
		if ($("#WContacts").attr("Contact"+i)=="google")
			texthtml=texthtml + '<a href="http://plus.google.com/'+$(".WContacts").attr("ID"+i)+'"><img class="contact" src="Pics/Contacts/googleplus.png"></a>';
		if ($("#WContacts").attr("Contact"+i)=="wordpress")
			texthtml=texthtml + '<a href="http://www.wordpress.com/'+$(".WContacts").attr("ID"+i)+'"><img class="contact" src="Pics/Contacts/wordpress.png"></a>';
	}
	return texthtml;
}