<!DOCTYPE html>
<html lang="en">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <link rel="shortcut icon" href="/favicon.ico" />
    <link rel="stylesheet" type="text/css" href="/css/ui-lightness/jquery-ui-1.10.3.css?v=<?= define::VERSION; ?>" />
    <link rel="stylesheet" type="text/css" media="screen" href="/css/jstree-3.0.6/style.css?v=<?= define::VERSION; ?>">
<!--    <link rel="stylesheet" type="text/css" href="/css/jquery.dataTables-1.9.4.css?v=<?= define::VERSION; ?>" />-->
    <link rel="stylesheet" type="text/css" href="/css/bootstrap-3.1.1.css?v=<?= define::VERSION; ?>" />
    <link rel="stylesheet" type="text/css" href="/css/bootstrap-theme-3.1.1.css?v=<?= define::VERSION; ?>" />
    <link rel="stylesheet" type="text/css" href="/css/morris-0.4.3.css?v=<?= define::VERSION; ?>" />
    <link rel="stylesheet" type="text/css" href="/css/layout.css?v=<?= define::VERSION; ?>" />
    <link rel="stylesheet" type="text/css" href="/css/style.css?v=<?= define::VERSION; ?>" />
    <script type="text/javascript" src="/js/raphael-2.1.2.js?v=<?= define::VERSION; ?>"></script>
    <script type="text/javascript" src="/js/jquery-2.1.1.js?v=<?= define::VERSION; ?>"></script>
    <script type="text/javascript" src="/js/jquery-ui-1.11.2.js?v=<?= define::VERSION; ?>"></script>
    <script type="text/javascript" src="/js/jquery.dataTables-1.9.4.js?v=<?= define::VERSION; ?>"></script>
    <script type="text/javascript" src="/js/jquery.dataTables.ColReorder-1.0.8.js?v=<?= define::VERSION; ?>"></script>
<!--    <script type="text/javascript" src="/js/jquery.dataTables.FixedColumns-2.0.3.js?v=<?= define::VERSION; ?>"></script>
    <script type="text/javascript" src="/js/jquery.dataTables.FixedHeader-2.0.6.js?v=<?= define::VERSION; ?>"></script>-->
    <script type="text/javascript" src="/js/jquery.dataTables.sort.js?v=<?= define::VERSION; ?>"></script>
    <script type="text/javascript" src="/js/bootstrap-3.1.1.js?v=<?= define::VERSION; ?>"></script>
    <script type="text/javascript" src="/js/morris-0.4.3.js?v=<?= define::VERSION; ?>"></script>
    <script type="text/javascript" src="/js/jquery.jstree-3.0.6.js"></script>
    <script type="text/javascript" src="/js/xi.strip.js?v=<?= define::VERSION; ?>"></script>
    <script type="text/javascript" src="/js/xi.is.js?v=<?= define::VERSION; ?>"></script>
    <script type="text/javascript" src="/js/xi.html.js?v=<?= define::VERSION; ?>"></script>
    <?= !empty($head) ? $head : ""; ?>
    <?= !empty($javascript) ? $javascript : ""; ?>
    <title><?= !empty($title) ? $title : ""; ?></title>
</head>
<body onload="xi.html.dynamicFormElements(document);">
<?= !empty($content) ? $content : ""; ?>

<?= controller::render($this->template_dir() . "debug.inc") ?>
</body>
</html>
