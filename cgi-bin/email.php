#!/usr/bin/php-cgi

<?php
  // Get the form data from the HTTP POST request
  $name = $_POST['name'];
  $email = $_POST['email'];
  $message = $_POST['message'];

  // Validate the input data
  if (empty($name) || empty($email) || empty($message)) {
    die("Error: All fields are required.");
  }

  // Send an email with the form data
  $to = 'recipient@example.com';
  $subject = 'New message from website';
  $body = "Name: $name\nEmail: $email\nMessage: $message";
  $headers = "From: $email\r\nReply-To: $email\r\n";

  if (!mail($to, $subject, $body, $headers)) {
    die("Error: Failed to send email.");
  }

  // Redirect the user to a thank-you page
  header('Location: /thank-you.html');
?>
