<?php
include './Classes/PHPExcel.php';

$inputFileType = 'Excel2007';
$inputFileName = 'SBTreport-5003.xlsx';
$outputFileType = 'HTML';
$outputFileName = 'php://output';

// $objReader = PHPExcel_IOFactory::createReader($inputFileType);
// $objReader->setLoadAllSheets();
// $objPHPExcel = $objReader->load($inputFileName);
$objPHPExcel = PHPExcel_IOFactory::load($inputFileName);

$objPHPExcel->getActiveSheet()->setTitle(pathinfo($inputFileName, PATHINFO_BASENAME));
// $objPHPExcel->setActiveSheetIndex(1);
// $objPHPExcel->getSheet(1);
// $objPHPExcel->getActiveSheet();

$rendererName = PHPExcel_Settings::CHART_RENDERER_JPGRAPH;
$rendererLibrary = 'jpgraph3.5.0b1/src/';
$rendererLibraryPath = '/php/libraries/Charts/' . $rendererLibrary;

$objPHPExcelWriter = PHPExcel_IOFactory::createWriter($objPHPExcel,$outputFileType);
// $objPHPExcelWriter->setImagesRoot('http://10.37.116.111:8000');
$objPHPExcelWriter->writeAllSheets();
$objPHPExcelWriter->setIncludeCharts(TRUE);
$objPHPExcelWriter->setPreCalculateFormulas(true);
$objPHPExcelWriter->save($outputFileName);
?>
