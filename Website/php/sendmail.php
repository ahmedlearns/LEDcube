<?php if(!$_POST) exit;
	$to 		= "meedowi@gmail.com";
	$email 		= $_POST['email'];
	$name 		= $_POST['name'];
	$message	= $_POST['message'];
	
	$subject 	= "You've been contacted by $name";
	
	$content 	= "$name sent you a message from your enquiry form:\r\n\n";
	$content   .= "Contact Reason: $message \n\nEmail: $email \n\n";
	
	if(@mail($to, $subject, $content, "From: $email \r\n Reply-To: $email \r\nReturn-Path: $email\r\n")) {
		echo "<h5 class='success'>Message Sent</h5>";
        echo "<br/><p class='success'>Thank you <strong>$name</strong>, your message has been submitted and someone will contact you shortly.</p>";
	}else{
		echo "<h2 class='failure'>Sorry, Try again Later.</h2>";
	}?>