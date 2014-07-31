// JavaScript Document
function ContentEN(a)
   {
		Bookmarking(a);
		var i;

//		Bookmark_mouse_s(a);
	   if (a==1)
	   {
		   $(".tagline").text(Data.ENG.Summary.Tagline);

		   $(".heading").text(Data.ENG.Summary.Heading);
		   var texta = '<div id= "Summary">';
		   for (i=1;i<=Data.ENG.Summary.Count;i++)
		   {
			   texta = texta +'<span class="glyphicon glyphicon-ok"></span> ' + Data.ENG.Summary["Point"+i] + '<br>';
		   }
		   $(".text").html(texta);   
	   }
	   if (a==2)
	   {
		   $(".tagline").text(Data.ENG.Education.Tagline);
		   $(".heading").text(Data.ENG.Education.Heading);
		   CollapsibleTitleBegin = '<div class="panel"><h4><a data-toggle="collapse" data-parent="#accordion" href="#';
		   CollapsibleTitleEnd = '</div></a></h4><div id="';
		   CollapsibleTextBegin = '" class="collapse';
		   CollapsibleTextEnd = '</div></div>';
		   var texta = '<div id="accordion">';
		   for (i=1;i<=Data.ENG.Education.Count;i++)
		   {
			   texta=texta + CollapsibleTitleBegin + Data.ENG.Education["Edu"+i].Id + '"><div class="ClassHeader"><span class="glyphicon glyphicon-book"></span><b>' + Data.ENG.Education["Edu"+i].School + '</b>' + CollapsibleTitleEnd + Data.ENG.Education["Edu"+i].Id + CollapsibleTextBegin + '"><div class="normaltext">' + Data.ENG.Education["Edu"+i].Degree + '<br>' + Data.ENG.Education["Edu"+i].Desc + '<br><i>Professional Thesis</i>: "' + Data.ENG.Education["Edu"+i].Thesis + '" <br><i>' + Data.ENG.Education["Edu"+i].Keywords + '</i></div>' + CollapsibleTextEnd;
		   }
		   $(".text").html(texta);  
	   }
	   if (a==3)
	   {
		   $(".tagline").text(Data.ENG.Work.Tagline);
		   $(".heading").text(Data.ENG.Work.Heading);
		   CollapsibleTitleBegin = '<div class="panel"><h4><a data-toggle="collapse" data-parent="#accordion" href="#';
		   CollapsibleTitleEnd = '</div></a></h4><div id="';
		   CollapsibleTextBegin = '" class="collapse';
		   CollapsibleTextEnd = '</div></div>';
		   var texta = '<div id="accordion">';
		   for (i=1;i<=Data.ENG.Work.Count;i++)
		   {
			   texta = texta + CollapsibleTitleBegin + Data.ENG.Work["Work"+i].Id + '"><div class="ClassHeader"><span class="glyphicon glyphicon-book"/><b>' + Data.ENG.Work["Work"+i].Company + '</b>' + CollapsibleTitleEnd + Data.ENG.Work["Work"+i].Id + CollapsibleTextBegin + '"><div class="normaltext">' + Data.ENG.Work["Work"+i].Role + ',<i> ' + Data.ENG.Work["Work"+i].Department + ' </i><br>' + Data.ENG.Work["Work"+i].Desc1 + '<br>' + Data.ENG.Work["Work"+i].Desc2 + '<br><i>' + Data.ENG.Work["Work"+i].Keywords + '</i></div>' + CollapsibleTextEnd;
		   }
		   $(".text").html(texta);  
	   }
	   if (a==4)
	   {
		   $(".tagline").text(Data.ENG.Languages.Tagline);
		   $(".heading").text(Data.ENG.Languages.Heading);
		   texta = '<table class= "table table-bordered table-hover"><thead><tr class= "success"><th width:"20%">Language</th><th>Description</th></tr></thead><tbody>';
		   for (i=1;i<=Data.ENG.Languages.Count;i++)
		   {
				texta= texta + '<tr><td><b>' + Data.ENG.Languages["Lang"+i].Name + '</b></td><td>' + Data.ENG.Languages["Lang"+i].Desc + '</td></tr>';
		   }
		   texta= texta + '</tbody></table>';
		   $(".text").html(texta);  
	   }
	   if (a==5)
	   {
		   $(".tagline").text(Data.ENG.IT.Tagline);
		   $(".heading").text(Data.ENG.IT.Heading);
		   var tablerating = '<table class= "table table-bordered table-hover"><thead><tr class= "success"><th style="width:30%">Tool</th><th style="width:50%">Description</th><th style="width:15%">Years</th></tr></thead><tbody>';
		   texta= '<ul class="nav nav-tabs">';
		   for(i=1;i<=Data.ENG.IT.Count;i++)
		   {
			   texta = texta + '<li '+ ((i==1)?'class="active"':'') + '><a href="#' + Data.ENG.IT["Sector"+i].Id + '" data-toggle="tab">' + Data.ENG.IT["Sector"+i].Name + '</a></li>';
		   }
		   texta = texta +'</ul><div class="tab-content">';
		   for(i=1;i<=Data.ENG.IT.Count;i++)
		   {
			   texta = texta + '<div class="tab-pane '+ ((i==1)?'active':'') + '" id="' + Data.ENG.IT["Sector"+i].Id + '">' + tablerating;
			   for (j=1;j<=Data.ENG.IT["Sector"+i].Count;j++)
			   {
				   texta = texta + '<tr><td>' + Data.ENG.IT["Sector"+i]["Lang"+j].Name + '</td><td class="ITDesc">' + Data.ENG.IT["Sector"+i]["Lang"+j].Desc + '</td><td><span class="stars">' + Data.ENG.IT["Sector"+i]["Lang"+j].Rating + '</span></td></td></tr>'
			   }
			   texta = texta + '</tbody></table></div>';
		   }
		   $(".text").html(texta);  
		   $('span.stars').stars();
	   }
	   if (a==6)
	   {
		   $(".tagline").text(Data.ENG.Projects.Tagline);
		   $(".heading").text(Data.ENG.Projects.Heading);
		   texta= '<div class="row"><div class="btn-group" data-toggle="buttons" id="btn" style="width:100%;">';
		   for (i=1;i<=Data.ENG.Projects.Count;i++)
		   {
			   texta = texta + '<label class="btn btn-primary btn-lg" style="width:33%;" onClick="Projects('+i+');"><input type="radio" name="options" id="option'+i+'">'+Data.ENG.Projects["Sector"+i].Name+' Projects</label>';
		   }

		   texta = texta + '</div></div><div id="ProjectText"></div>';
		   $(".text").html(texta);
	   }
	   if (a==7)
	   {
		   $(".heading").text(Data.ENG.Hobbies.Heading);
		   $(".tagline").text(Data.ENG.Hobbies.Tagline);
		   CollapsibleTitleBegin = '<div class="panel"><h4><a data-toggle="collapse" data-parent="#accordion" href="#';
		   CollapsibleTitleEnd = '</div></a></h4><div id="';
		   CollapsibleTextBegin = '" class="collapse';
		   CollapsibleTextEnd = '</div></div>';
		   var texta = '<div id="accordion">';
		   var HKey,HLink;
		   for (i=1;i<Data.ENG.Hobbies.Count;i++)
		   {
			   texta = texta + CollapsibleTitleBegin + Data.ENG.Hobbies["Hobby"+i].Id + '"><div class="ClassHeader"><span class="glyphicon glyphicon-book"/><b>' + Data.ENG.Hobbies["Hobby"+i].Name + '</b>' + CollapsibleTitleEnd + Data.ENG.Hobbies["Hobby"+i].Id + CollapsibleTextBegin + '"><div class="normaltext">' + Data.ENG.Hobbies["Hobby"+i].Desc;
			   HKey = (Data.ENG.Hobbies["Hobby"+i].Keywords!='undefined')?('<br><i>' + Data.ENG.Hobbies["Hobby"+i].Keywords + '</i>'):'';
			   HLink = (Data.ENG.Hobbies["Hobby"+i].Link)!=""?('<br><i>' + Data.ENG.Hobbies["Hobby"+i].Link + '</i>'):'';
			   texta = texta + HKey + HLink + '</div>' + CollapsibleTextEnd;
		   }
		   $(".text").html(texta);  
	   }
	   	if (a==8)
	   {
		   $(".heading").text(Data.ENG.CV.Heading);
		   $(".tagline").text(Data.ENG.CV.Tagline);
		   texta = '<table class= "table CVtable table-bordered table-hover"><thead><tr class= "success"><th width:"20%">CV Language</th><th>CV Number of Pages</th></tr></thead><tbody>';
		   for (i=1;i<=Data.ENG.CV.Count;i++)
		   {
			   texta= texta + '<tr class= "active" OnClick="CVContent('+i+');"><td><b>'+Data.ENG.CV["CV"+i].Language+'</b></td><td>'+Data.ENG.CV["CV"+i].Length+'</td></tr>';
		   }
		   texta=texta+'</tbody></table>';
		   $(".text").html(texta);   
	   }
   }
function CVContent(a)
{
	   $(".heading").text(Data.ENG.CV["CV"+a].Name);
	   texta = '<p  style=" margin: 12px auto 6px auto; font-family: Helvetica,Arial,Sans-serif; font-style: normal; font-variant: normal; font-weight: normal; font-size: 14px; line-height: normal; font-size-adjust: none; font-stretch: normal; -x-system-font: none; display: block;">';
	   texta = texta + Data.ENG.CV["CV"+a].Link
	   $(".text").html(texta);   
}
function Projects(a)
{
	   CollapsibleTitleBegin = '<div class="panel"><h4><a data-toggle="collapse" data-parent="#accordion" href="#';
	   CollapsibleTitleEnd = '</div></a></h4><div id="';
	   CollapsibleTextBegin = '" class="collapse';
	   CollapsibleTextEnd = '</div></div>';
	   var ProjectText = '<div id="accordion">';
	   for (i=1;i<Data.ENG.Projects["Sector"+a].Count;i++)
	   {
		   ProjectText = ProjectText+ CollapsibleTitleBegin +Data.ENG.Projects["Sector"+a]["Project"+i].Id+'"><div class="ClassHeader"><span class="glyphicon glyphicon-book"></span><b>'+Data.ENG.Projects["Sector"+a]["Project"+i].Name+'</b>' + CollapsibleTitleEnd + Data.ENG.Projects["Sector"+a]["Project"+i].Id + CollapsibleTextBegin + '"><div class="normaltext">'+Data.ENG.Projects["Sector"+a]["Project"+i].Desc;
		   Thesis = (Data.ENG.Projects["Sector"+a]["Project"+i].Thesis!="")?('<br><i>Professional Thesis</i>: "'+Data.ENG.Projects["Sector"+a]["Project"+i].Thesis):""; 
		   ProjectText = ProjectText + Thesis;
		   ProjectText = ProjectText + '" <br><i>'+Data.ENG.Projects["Sector"+a]["Project"+i].KeyWords+'</i></div>' + CollapsibleTextEnd;
	   }
	   $("#ProjectText").html(ProjectText);  
}