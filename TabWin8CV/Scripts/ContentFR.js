// JavaScript Document
function ContentFR(a)
   {
		Bookmarking(a);
		

//		Bookmark_mouse_s(a);
	   if (a==1)
	   {
		   $(".tagline").text("Implement my skills and passion for cutting-edge technology in a dynamic and innovative environment");

		   $(".heading").text("Objectif");
		   var texta = '<span class="glyphicon glyphicon-ok">  </span>Conseil en Gestion de Projet et Ingenièrie<br>';
		   texta = texta +'<span class="glyphicon glyphicon-ok">  </span>Experience Professional dans les industrie High-Tech comme Airbus<br>';
		   texta = texta +'<span class="glyphicon glyphicon-ok">  </span>Études avancées en Gestion de Projet et Genie Aerospatiale<br>';
		   texta = texta +'<span class="glyphicon glyphicon-ok">  </span>Competence diversifiée en Developpement du logiciel et Web<br>';
		   texta = texta +'<span class="glyphicon glyphicon-ok">  </span>Novatrice, Technique et Analytique esprit<br>';
		   texta = texta +'<span class="glyphicon glyphicon-ok">  </span>Experience internationale en Inde, France et Canada<br>';
		   texta = texta +'<span class="glyphicon glyphicon-ok">  </span>Competences Linguistiques en Anglais, Hindi, Français, Allemand...</div>';
		   $(".text").html(texta);
	   }
	   if (a==2)
	   {
		   $(".tagline").text("Double Masters in Aerospace Project Management and Aerospace Engineering");
		   $(".heading").text("Formation");
		   CollapsibleTitleBegin = '<div class="panel"><h4><a data-toggle="collapse" data-parent="#accordion" href="#';
		   CollapsibleTitleEnd = '</div></a></h4><div id="';
		   CollapsibleTextBegin = '" class="collapse';
		   CollapsibleTextEnd = '</div></div>';
		   var texta = '<div id="accordion">' + CollapsibleTitleBegin +'APM"><div class="ClassHeader"><span class="glyphicon glyphicon-book"></span><b>ISAE (Supaero), ENAC et Ecole de l\'air (France)</b>' + CollapsibleTitleEnd +'APM' + CollapsibleTextBegin + ' in"><div class="normaltext">Master spécialisé en Gestion de Projet Aérospatial <br>Maîtrise mettant l\'accent sur les tendances actuelles dans l\'industrie aérospatiale : <br><i>Objet de la thèse professionnelle : </i> "Evolution of Project Management Software in large industries" <br><i>Stratégie, budget, investissements, planification, connaissances en management, risques.. </i></div>' + CollapsibleTextEnd;
		   texta = texta + CollapsibleTitleBegin +'PMI"><div class="ClassHeader"><span class="glyphicon glyphicon-book">  </span><b> Project Management Institute (PMI)</b>' + CollapsibleTitleEnd +'PMI' + CollapsibleTextBegin + '"><div class="normaltext">Certified Associate in Project Management (CAPM)<br>Certification de niveau d’entrée pour les gestionnaires de projet, qui atteste des connaissances fondamentales, des terminologies et des processus, d’une gestion de projet efficace.  </div>' + CollapsibleTextEnd;
		   texta = texta + CollapsibleTitleBegin +'IIT"><div class="ClassHeader"><span class="glyphicon glyphicon-book">  </span><b>Institut Indien de Technologie Bombay - Mumbai, Inde</b>' + CollapsibleTitleEnd +'IIT' + CollapsibleTextBegin + '"><div class="normaltext">Génie Aérospatial (M.Tech. et B. Tech)   <br>Expertise dans la plusieurs secteur d\'industrie aérospatiale avec stage à l\'ISAE, France <br><i>Objet de la thèse professionnelle : </i> “Application of PSO in Aircraft Conceptual Design”  <br><i>Optimisation multidisciplinaire, Conception d’aéronefs, Commande du système,  Traitement des signaux </i></div>' + CollapsibleTextEnd;
		   texta = texta + CollapsibleTitleBegin +'UOT"><div class="ClassHeader"><span class="glyphicon glyphicon-book">  </span><b> University de Toronto, Canada </b>' + CollapsibleTitleEnd +'UOT' + CollapsibleTextBegin + '"><div class="normaltext">Echange Etudiant<br>Conception de la structure des aéronefs et de la langue allemande <br><i>Objet de la thèse professionnelle :</i> : “2D and 3D Object Detection and Tracking/Human Machine Interaction”  <br><i>Aéroélasticité, Vibrations des structures, Robotique, Conception et la calcul de drones, Allemand </i></div></div>' + CollapsibleTextEnd;
		   $(".text").html(texta);  
	   }
	   if (a==3)
	   {
		   $(".tagline").text("Experience as a Project Management consultant and a software developer");
		   $(".heading").text("Expérience Professionnelle");
		   CollapsibleTitleBegin = '<div class="panel"><h4><a data-toggle="collapse" data-parent="#accordion" href="#';
		   CollapsibleTitleEnd = '</div></a></h4><div id="';
		   CollapsibleTextBegin = '" class="collapse';
		   CollapsibleTextEnd = '</div></div>';
		   var texta = '<div id="accordion">' + CollapsibleTitleBegin +'AIRBUS"><div class="ClassHeader"><span class="glyphicon glyphicon-book"/><b>Airbus (France)</b>' + CollapsibleTitleEnd +'AIRBUS' + CollapsibleTextBegin + ' in"><div class="normaltext">Project Management Trainee, <i>Service à la clientèle Exploitation et Support Training</i> <br>Responsable de l\'introduction de méthodologies de gestion de projet de Project Portfolio Transverse <br>Développement de l\'interface Web pour le multi Programme Projets de collaboration d\'équipe<br><i>Transversal normalisation, Microsoft SharePoint, Business Improvement, Web Design, Macro, MS Office </i></div>' + CollapsibleTextEnd;
		   texta = texta + CollapsibleTitleBegin +'MEDUSIND"><div class="ClassHeader"><span class="glyphicon glyphicon-book"/><b>Medusind Solutions (Inde)</b>' + CollapsibleTitleEnd +'MEDUSIND' + CollapsibleTextBegin + '"><div class="normaltext">Software Developer,<i> Healthcare Software Solutions </i><br>Responsable de l\'automatisation et de gestion de base de données pour les services à la clientèle<br><i>Gestion de projet, l\'automatisation, l\'analyse, VB,. Net 3.5, MS Office, SQL, Macro-programmation</i></div>' + CollapsibleTextEnd;
		   texta = texta + CollapsibleTitleBegin +'ENTREP"><div class="ClassHeader"><span class="glyphicon glyphicon-book"/><b> 3O Technologies (Inde) </b>' + CollapsibleTitleEnd +'ENTREP' + CollapsibleTextBegin + '"><div class="normaltext">Co-Founder <i>(Entrepreneur)</i><br>Outil logiciel pour Global Market Forecast pour Airbus Freighter marketing<br><i>Modélisation d\'affaires, Négociation, Compagnie aérienne analyse des besoins, Logistique, Marketing, Création de valeur, Algorithmes complexes, Résolution des problèmes analytiques, Etudes de marché, Visual C + +  </i></div></div>' + CollapsibleTextEnd;
		   
		   $(".text").html(texta);  
	}
	   if (a==4)
	   {
		    $(".tagline").text("Multi-lingual with a focus on European Languages and culture");
		   $(".heading").text("Language Profile");
		   texta = '<table class= "table table-bordered table-hover"><thead><tr class= "success"><th>Langue</th><th>Basic</th><th>Conversation</th><th>Bilingue</th></tr></thead><tbody>';
		   texta= texta + '<tr class= "active"><td><b>Anglais</b></td><td></td><td></td><td><span class="glyphicon glyphicon-check">  </span></td></tr>';
		   texta= texta + '<tr class= "active"><td><b>Hindi</b></td><td></td><td></td><td><span class="glyphicon glyphicon-check">  </span></td></tr>';
		   texta= texta + '<tr class= "active"><td><b>Français</b></td><td></td><td><span class="glyphicon glyphicon-check">  </span></td><td></td></tr>';
		   texta= texta + '<tr class= "active"><td><b>Allemand</b></td><td><span class="glyphicon glyphicon-check">  </span></td><td></td><td></td></tr></tbody></table>';
		   $(".text").html(texta);  
	   }
	   if (a==5)
	   {
		   $(".tagline").text("Diverse experience in major programming languages, OS, Softwares and Utilities");
		   $(".heading").text("Compétences Informatiques");
		   var tablerating = '<table class= "table table-bordered table-hover"><thead><tr class= "success"><th style="width:30%">Outil</th><th style="width:50%">Description</th><th style="width:15%">Rating</th></tr></thead><tbody>';
		   texta= '<ul class="nav nav-tabs"><li class="active"><a href="#Programming" data-toggle="tab">Coding</a></li><li><a href="#WebDesign" data-toggle="tab">Web-Design</a></li><li><a href="#PMTools" data-toggle="tab">Management</a></li><li><a href="#IT" data-toggle="tab">Database</a></li><li><a href="#Engg" data-toggle="tab">Engineering</a></li><li><a href="#Toolkits" data-toggle="tab">Processes</a></li></ul>'
		   texta= texta+ '<!-- Tab panes --><div class="tab-content"><div class="tab-pane active" id="Programming">' + tablerating + '<tr><td>Java</td><td></td><td><span class="stars">3</span></td></tr><tr><td>C/C++</td><td></td><td><span class="stars">4.5</span></td></td></tr><tr><td>Visual Basic</td><td></td><td><span class="stars">4</span></td></td></tr><tr><td>FORTRAN 90</td><td></td><td><span class="stars">4</span></td></td></tr><tr><td>OpenCV</td><td></td><td><span class="stars">3.5</span></td></td></tr><tr><td>MATLAB</td><td></td><td><span class="stars">5</span></td></td></tr></tbody></table></div>';
		   texta= texta+ '<div class="tab-pane" id="WebDesign">' + tablerating + '<tr><td>HTML</td><td></td><td><span class="stars">5</span></td></td></tr><tr><td>XML</td><td></td><td><span class="stars">3</span></td></td></tr><tr><td>Javascript</td><td></td><td><span class="stars">4.5</span></td></td></tr><tr><td>PHP</td><td></td><td><span class="stars">3</span></td></td></tr><tr><td>VBScript</td><td></td><td><span class="stars">4</span></td></td></tr><tr><td>Dreamweaver</td><td></td><td><span class="stars">4</span></td></td></tr></tbody></table></div>'
		   texta= texta+ '<div class="tab-pane" id="PMTools">' + tablerating + '<tr><td>MS Project</td><td></td><td><span class="stars">4.5</span></td></td></tr><tr><td>MS Sharepoint 2010/13</td><td></td><td><span class="stars">4</span></td></td></tr><tr><td>Primavera</td><td></td><td><span class="stars">3</span></td></td></tr><tr><td>SAP</td><td></td><td><span class="stars">2</span></td></td></tr></tbody></table></div>';
			texta= texta+ '<div class="tab-pane" id="IT">' + tablerating + '<tr><td>MS Office Pack</td><td></td><td><span class="stars">5</span></td></td></tr><tr><td> Open Office</td><td></td><td><span class="stars">4</span></td></td></tr><tr><td>VBA Macros</td><td></td><td><span class="stars">5</span></td></td></tr><tr><td>SQL</td><td></td><td><span class="stars">3</span></td></td></tr></tbody></table></div>';
			texta= texta+ '<div class="tab-pane" id="Toolkits">' + tablerating + '<tr><td>Airbus GPP</td><td></td><td><span class="stars">4</span></td></td></tr><tr><td> Airbus LBIP</td><td></td><td><span class="stars">5</span></td></td></tr><tr><td>UML</td><td></td><td><span class="stars">3.5</span></td></td></tr><tr><td>DO178B</td><td></td><td><span class="stars">3</span></td></td></tr></tbody></table></div>';
		   texta= texta+ '<div class="tab-pane" id="Engg">' + tablerating + '<tr><td>Catia V5</td><td></td><td><span class="stars">3</span></td></td></tr><tr><td>Solidworks/13</td><td></td><td><span class="stars">2.5</span></td></td></tr><tr><td>LabView</td><td></td><td><span class="stars">3.5</span></td></td></tr><tr><td>Simulink</td><td></td><td><span class="stars">4</span></td></td></tr></tbody></table></div>';
		   $(".text").html(texta);  
		   $('span.stars').stars();
	   }
	   if (a==6)
	   {
		   $(".tagline").text("Multicultural Team and Individual Projects in India, Canada and France");
		   $(".heading").text("Projets");
		   texta= 'Coming soon...';
		   $(".text").html(texta);
	   }
	   if (a==7)
	   {
		   $(".heading").text("Activités parascolaires");
		$(".tagline").text("Under Construction");
		   texta= 'Coming soon...';
		   $(".text").html(texta);  
	   }
   }function Content(a)
   {
		Bookmarking(a);
		

//		Bookmark_mouse_s(a);
	   if (a==1)
	   {
		   $(".tagline").text("Implement my skills and passion for cutting-edge technology in a dynamic and innovative environment");

		   $(".heading").text("Professional Profile");
		   var texta = '<span class="glyphicon glyphicon-ok">  </span>Project Management and Engineering Consultant<br>';
		   texta = texta +'<span class="glyphicon glyphicon-ok">  </span>Work experience in high tech enterprise like Airbus<br>';
		   texta = texta +'<span class="glyphicon glyphicon-ok">  </span>Advanced studies in Aerospace Engineering and Management<br>';
		   texta = texta +'<span class="glyphicon glyphicon-ok">  </span>Diverse skills in Software and web development<br>';
		   texta = texta +'<span class="glyphicon glyphicon-ok">  </span>Problem Solving, Technical and Innovative Approach<br>';
		   texta = texta +'<span class="glyphicon glyphicon-ok">  </span>Multicultural experience in India, France and Canada<br>';
		   texta = texta +'<span class="glyphicon glyphicon-ok">  </span>Polyglot with skills in English, Hindi, French, German...</div>';
		   $(".text").html(texta);
	   }
	   if (a==2)
	   {
		   $(".tagline").text("Double Masters in Aerospace Project Management and Aerospace Engineering");
		   $(".heading").text("Education Profile");
		   CollapsibleTitleBegin = '<div class="panel"><h4><a data-toggle="collapse" data-parent="#accordion" href="#';
		   CollapsibleTitleEnd = '</div></a></h4><div id="';
		   CollapsibleTextBegin = '" class="collapse';
		   CollapsibleTextEnd = '</div></div>';
		   var texta = '<div id="accordion">' + CollapsibleTitleBegin +'APM"><div class="ClassHeader"><span class="glyphicon glyphicon-book"></span><b>ISAE (Supaero), ENAC and Ecole de l\'air (France)</b>' + CollapsibleTitleEnd +'APM' + CollapsibleTextBegin + ' in"><div class="normaltext">Professional Masters in Aerospace Project Management <br>Special Diploma focusing on current trends in the Aerospace Industry<br><i>Professional Thesis</i>: "Evolution of Project Management Software in large industries" <br><i>Strategy, Budget, Investments, Planning, Knowledge Management, Risks, Diversity Management </i></div>' + CollapsibleTextEnd;
		   texta = texta + CollapsibleTitleBegin +'PMI"><div class="ClassHeader"><span class="glyphicon glyphicon-book">  </span><b> Project Management Institute (PMI)</b>' + CollapsibleTitleEnd +'PMI' + CollapsibleTextBegin + '"><div class="normaltext">Certified Associate in Project Management (CAPM)<br>Entry-level certification for project practitioners to demonstrate understanding of the fundamental knowledge, terminology and processes of effective project management</div>' + CollapsibleTextEnd;
		   texta = texta + CollapsibleTitleBegin +'IIT"><div class="ClassHeader"><span class="glyphicon glyphicon-book">  </span><b> Indian Institute of Technology Bombay - Mumbai, India</b>' + CollapsibleTitleEnd +'IIT' + CollapsibleTextBegin + '"><div class="normaltext">Aerospace Engineering (M.Tech. & B. Tech)  <br>Expertise in several sectors of Aerospace Industry with an internship at ISAE, France<br><i>Professional Thesis</i> : “Application of PSO in Aircraft Conceptual Design”  <br><i>Multidisciplinary Optimisation,Aircraft Design and Analysis,Avionics, Image Processing </i></div>' + CollapsibleTextEnd;
		   texta = texta + CollapsibleTitleBegin +'UOT"><div class="ClassHeader"><span class="glyphicon glyphicon-book">  </span><b> University of Toronto, Canada </b>' + CollapsibleTitleEnd +'UOT' + CollapsibleTextBegin + '"><div class="normaltext">Exchange Student<br>Aircraft Structural Design and Fabrication and German Language <br><i>Professional Thesis</i> : “2D and 3D Object Detection and Tracking/Human Machine Interaction”  <br><i>Aeroelasticity, Structural Vibrations, Robotics, Conception and Design of UAV, German </i></div></div>' + CollapsibleTextEnd;
		   $(".text").html(texta);  
	   }
	   if (a==3)
	   {
		   $(".tagline").text("Experience as a Project Management consultant and a software developer");
		   $(".heading").text("Work Profile");
		   CollapsibleTitleBegin = '<div class="panel"><h4><a data-toggle="collapse" data-parent="#accordion" href="#';
		   CollapsibleTitleEnd = '</div></a></h4><div id="';
		   CollapsibleTextBegin = '" class="collapse';
		   CollapsibleTextEnd = '</div></div>';
		   var texta = '<div id="accordion">' + CollapsibleTitleBegin +'AIRBUS"><div class="ClassHeader"><span class="glyphicon glyphicon-book"/><b>Airbus (France)</b>' + CollapsibleTitleEnd +'AIRBUS' + CollapsibleTextBegin + ' in"><div class="normaltext">Project Management Trainee, <i>Customer Services Operations & Training Support</i> <br>Responsible for introducing Project Management methodologies to Transverse Project Portfolio Development of Web interface for Multi Program Team Collaboration Projects<br><i>Transverse Standardization, Microsoft SharePoint, Business Improvement, Web Design, Macro, MS Office</i></div>' + CollapsibleTextEnd;
		   texta = texta + CollapsibleTitleBegin +'MEDUSIND"><div class="ClassHeader"><span class="glyphicon glyphicon-book"/><b>Medusind Solutions (India)</b>' + CollapsibleTitleEnd +'MEDUSIND' + CollapsibleTextBegin + '"><div class="normaltext">Software Developer,<i> Healthcare Software Solutions </i><br>Responsible for automation and management of database for Customer Services, development of Text Parsing, Database handling, Data Monitoring tools<br><i>Project Management, Automation, Parsing, VB, .Net 3.5, MS Office, SQL, Macro-Programming</i></div>' + CollapsibleTextEnd;
		   texta = texta + CollapsibleTitleBegin +'ENTREP"><div class="ClassHeader"><span class="glyphicon glyphicon-book"/><b> 3O Technologies (India) </b>' + CollapsibleTitleEnd +'ENTREP' + CollapsibleTextBegin + '"><div class="normaltext">Co-Founder <i>(Entrepreneurial Venture)</i><br>Software consulting for Global Market Forecast for Airbus Freighter Marketing<br><i>Business modeling, Negotiation, Airline requirements Analysis, Logistics, Finance, Marketing, Value Creation, Complex Algorithms, Analytical Problem solving, Market research, Visual C++ </i></div></div>' + CollapsibleTextEnd;
		   
		   $(".text").html(texta);  
	}
	   if (a==4)
	   {
		    $(".tagline").text("Multi-lingual with a focus on European Languages and culture");
		   $(".heading").text("Language Profile");
		   texta = '<table class= "table table-bordered table-hover"><thead><tr class= "success"><th>Language</th><th>Basic</th><th>Conversational</th><th>Bilingual</th></tr></thead><tbody>';
		   texta= texta + '<tr class= "active"><td><b>English</b></td><td></td><td></td><td><span class="glyphicon glyphicon-check">  </span></td></tr>';
		   texta= texta + '<tr class= "active"><td><b>Hindi</b></td><td></td><td></td><td><span class="glyphicon glyphicon-check">  </span></td></tr>';
		   texta= texta + '<tr class= "active"><td><b>French</b></td><td></td><td><span class="glyphicon glyphicon-check">  </span></td><td></td></tr>';
		   texta= texta + '<tr class= "active"><td><b>German</b></td><td><span class="glyphicon glyphicon-check">  </span></td><td></td><td></td></tr></tbody></table>';
		   $(".text").html(texta);  
	   }
	   if (a==5)
	   {
		   $(".tagline").text("Diverse experience in major programming languages, OS, Softwares and Utilities");
		   $(".heading").text("IT Profile");
		   var tablerating = '<table class= "table table-bordered table-hover"><thead><tr class= "success"><th style="width:30%">Tool</th><th style="width:50%">Description</th><th style="width:15%">Rating</th></tr></thead><tbody>';
		   texta= '<ul class="nav nav-tabs"><li class="active"><a href="#Programming" data-toggle="tab">Coding</a></li><li><a href="#WebDesign" data-toggle="tab">Web-Design</a></li><li><a href="#PMTools" data-toggle="tab">Management</a></li><li><a href="#IT" data-toggle="tab">Database</a></li><li><a href="#Engg" data-toggle="tab">Engineering</a></li><li><a href="#Toolkits" data-toggle="tab">Processes</a></li></ul>'
		   texta= texta+ '<!-- Tab panes --><div class="tab-content"><div class="tab-pane active" id="Programming">' + tablerating + '<tr><td>Java</td><td></td><td><span class="stars">3</span></td></tr><tr><td>C/C++</td><td></td><td><span class="stars">4.5</span></td></td></tr><tr><td>Visual Basic</td><td></td><td><span class="stars">4</span></td></td></tr><tr><td>FORTRAN 90</td><td></td><td><span class="stars">4</span></td></td></tr><tr><td>OpenCV</td><td></td><td><span class="stars">3.5</span></td></td></tr><tr><td>MATLAB</td><td></td><td><span class="stars">5</span></td></td></tr></tbody></table></div>';
		   texta= texta+ '<div class="tab-pane" id="WebDesign">' + tablerating + '<tr><td>HTML</td><td></td><td><span class="stars">5</span></td></td></tr><tr><td>XML</td><td></td><td><span class="stars">3</span></td></td></tr><tr><td>Javascript</td><td></td><td><span class="stars">4.5</span></td></td></tr><tr><td>PHP</td><td></td><td><span class="stars">3</span></td></td></tr><tr><td>VBScript</td><td></td><td><span class="stars">4</span></td></td></tr><tr><td>Dreamweaver</td><td></td><td><span class="stars">4</span></td></td></tr></tbody></table></div>'
		   texta= texta+ '<div class="tab-pane" id="PMTools">' + tablerating + '<tr><td>MS Project</td><td></td><td><span class="stars">4.5</span></td></td></tr><tr><td>MS Sharepoint 2010/13</td><td></td><td><span class="stars">4</span></td></td></tr><tr><td>Primavera</td><td></td><td><span class="stars">3</span></td></td></tr><tr><td>SAP</td><td></td><td><span class="stars">2</span></td></td></tr></tbody></table></div>';
			texta= texta+ '<div class="tab-pane" id="IT">' + tablerating + '<tr><td>MS Office Pack</td><td></td><td><span class="stars">5</span></td></td></tr><tr><td> Open Office</td><td></td><td><span class="stars">4</span></td></td></tr><tr><td>VBA Macros</td><td></td><td><span class="stars">5</span></td></td></tr><tr><td>SQL</td><td></td><td><span class="stars">3</span></td></td></tr></tbody></table></div>';
			texta= texta+ '<div class="tab-pane" id="Toolkits">' + tablerating + '<tr><td>Airbus GPP</td><td></td><td><span class="stars">4</span></td></td></tr><tr><td> Airbus LBIP</td><td></td><td><span class="stars">5</span></td></td></tr><tr><td>UML</td><td></td><td><span class="stars">3.5</span></td></td></tr><tr><td>DO178B</td><td></td><td><span class="stars">3</span></td></td></tr></tbody></table></div>';
		   texta= texta+ '<div class="tab-pane" id="Engg">' + tablerating + '<tr><td>Catia V5</td><td></td><td><span class="stars">3</span></td></td></tr><tr><td>Solidworks/13</td><td></td><td><span class="stars">2.5</span></td></td></tr><tr><td>LabView</td><td></td><td><span class="stars">3.5</span></td></td></tr><tr><td>Simulink</td><td></td><td><span class="stars">4</span></td></td></tr></tbody></table></div>';
		   $(".text").html(texta);  
		   $('span.stars').stars();
	   }
	   if (a==6)
	   {
		   $(".tagline").text("Multicultural Team and Individual Projects in India, Canada and France");
		   $(".heading").text("Project Profile");
		   texta= 'Coming soon...';
		   $(".text").html(texta);
	   }
	   if (a==7)
	   {
		   $(".heading").text("Extracurricular Profile");
		$(".tagline").text("Under Construction");
		   texta= 'Coming soon...';
		   $(".text").html(texta);  
	   }
   }