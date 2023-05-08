<?php
// Import PHPMailer library
use PHPMailer\PHPMailer\PHPMailer;
use PHPMailer\PHPMailer\Exception;

// Include PHPMailer autoloader
require 'vendor/autoload.php';

// Create a new PHPMailer object
$mail = new PHPMailer(true);

try {
  // SMTP server settings
  $mail->isSMTP();
  $mail->Host       = 'smtp.naver.com';
  $mail->SMTPAuth   = true;
  $mail->Username   = $_GET['from']
  $mail->Password   = $_GET['password']
  $mail->SMTPSecure = PHPMailer::ENCRYPTION_STARTTLS;
  $mail->Port       = 465;

  // Sender and recipient email addresses
  $mail->setFrom($_GET['from'], $_GET['from']);
  $mail->addAddress($_GET['to'], $_GET['to']);

  // Email subject and message
  $mail->Subject = $_GET['subject'];
  $mail->Body    = $_GET['content'];

  // Send the email
  $mail->send();
  echo 'Email sent successfully';
} catch (Exception $e) {
  echo "Email could not be sent. Error message: {$mail->ErrorInfo}";
}
?>
