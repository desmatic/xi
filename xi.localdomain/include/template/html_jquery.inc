<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <link rel="shortcut icon" href="/favicon.ico" />
    <link rel="stylesheet" type="text/css" href="/css/ui-lightness/jquery-ui-1.10.3.css?v=<?= $version; ?>" />
<!--    <link rel="stylesheet" type="text/css" href="/css/jquery.dataTables-1.9.4.css?v=<?= $version; ?>" />-->
<!--    <link rel="stylesheet" type="text/css" href="/css/bootstrap-3.0.3.css?v=<?= $version; ?>" />-->
<!--    <link rel="stylesheet" type="text/css" href="/css/bootstrap-theme-3.0.3.css?v=<?= $version; ?>" />-->
<!--    <link rel="stylesheet" type="text/css" href="/css/morris-0.4.3.css?v=<?= $version; ?>" />-->
<!--    <link rel="stylesheet" type="text/css" href="/css/layout.css?v=<?= $version; ?>" />-->
<!--    <link rel="stylesheet" type="text/css" href="/css/style.css?v=<?= $version; ?>" />-->
<!--    <script type="text/javascript" src="/js/raphael-2.1.2.js?v=<?= $version; ?>"></script>-->
    <script type="text/javascript" src="/js/jquery-2.0.3.js?v=<?= $version; ?>"></script>
    <script type="text/javascript" src="/js/jquery-ui-1.10.3.js?v=<?= $version; ?>"></script>
    <script type="text/javascript" src="/js/jquery.dataTables-1.9.4.js?v=<?= $version; ?>"></script>
    <script type="text/javascript" src="/js/jquery.dataTables.ColReorder-1.0.8.js?v=<?= $version; ?>"></script>
<!--    <script type="text/javascript" src="/js/jquery.dataTables.FixedColumns-2.0.3.js?v=<?= $version; ?>"></script>
    <script type="text/javascript" src="/js/jquery.dataTables.FixedHeader-2.0.6.js?v=<?= $version; ?>"></script>-->
    <script type="text/javascript" src="/js/jquery.dataTables.sort.js?v=<?= $version; ?>"></script>
<!--    <script type="text/javascript" src="/js/bootstrap-3.0.3.js?v=<?= $version; ?>"></script>-->
<!--    <script type="text/javascript" src="/js/morris-0.4.3.js?v=<?= $version; ?>"></script>-->
    <script type="text/javascript" src="/js/xi.strip.js?v=<?= $version; ?>"></script>
    <script type="text/javascript" src="/js/xi.is.js?v=<?= $version; ?>"></script>
    <script type="text/javascript" src="/js/xi.html.js?v=<?= $version; ?>"></script>
    <?= $head; ?>
    <?= $javascript; ?>
    <title><?= $title; ?></title>
</head>
<body onload="xi.html.dynamicFormElements(document);">
<?= $content; ?>

<?= $debug; ?>
</body>
</html>
