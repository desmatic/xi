<!DOCTYPE html>
<html lang="en-us">
    <head>
        <meta charset="utf-8">
        <title><?= !empty($title) ? $title : ""; ?></title>
        <meta name="description" content="">
        <meta name="author" content="">
        <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
        
        <!-- jQuery dataTables -->
        <link rel="stylesheet" type="text/css" media="screen" href="/css/jquery.dataTables-1.10.4.css?v=<?= define::VERSION; ?>">
        <link rel="stylesheet" type="text/css" media="screen" href="/css/dataTables.colReorder-1.10.4.css?v=<?= define::VERSION; ?>">
        <link rel="stylesheet" type="text/css" media="screen" href="/css/dataTables.colVis-1.10.4.css?v=<?= define::VERSION; ?>">
        <link rel="stylesheet" type="text/css" media="screen" href="/css/dataTables.fixedColumns-1.10.4.css?v=<?= define::VERSION; ?>">
        <link rel="stylesheet" type="text/css" media="screen" href="/css/dataTables.tableTools-1.10.4.css?v=<?= define::VERSION; ?>">
        
        <!-- jQuery contextMenu -->
        <link rel="stylesheet" type="text/css" href="/css/jquery.contextMenu-1.6.6.css?v=<?= define::VERSION; ?>" />
        
        <!-- jQuery jsTree -->
        <link rel="stylesheet" type="text/css" media="screen" href="/css/jstree-3.0.6/style.css?v=<?= define::VERSION; ?>">
        
        <!-- jQuery-Guillotine -->
        <link rel="stylesheet" type="text/css" media="screen" href="/css/jquery.guillotine-1.3.1.css?v=<?= define::VERSION; ?>">
        
        <!-- Basic Styles -->
        <link rel="stylesheet" type="text/css" media="screen" href="/css/bootstrap-3.3.1.css?v=<?= define::VERSION; ?>">
        <link rel="stylesheet" type="text/css" media="screen" href="/css/bootstrap-theme-3.3.1.css?v=<?= define::VERSION; ?>">
        <link rel="stylesheet" type="text/css" media="screen" href="/css/smartadmin-bootstrap-1.52.css?v=<?= define::VERSION; ?>">
        <link rel="stylesheet" type="text/css" media="screen" href="/css/font-awesome-4.4.0.css?v=<?= define::VERSION; ?>">

        <!-- SmartAdmin Styles : Please note (smartadmin-production.css) was created using LESS variables -->
        <link rel="stylesheet" type="text/css" media="screen" href="/css/smartadmin-production-plugins-1.52.css?v=<?= define::VERSION; ?>">
        <link rel="stylesheet" type="text/css" media="screen" href="/css/smartadmin-production-1.52.css?v=<?= define::VERSION; ?>">
        <link rel="stylesheet" type="text/css" media="screen" href="/css/smartadmin-skins-1.52.css?v=<?= define::VERSION; ?>">
        <link rel="stylesheet" type="text/css" media="screen" href="/css/smartadmin-custom-1.0.css?v=<?= define::VERSION; ?>">
        
        <!-- XI -->
        <link rel="stylesheet" type="text/css" href="/css/layout.css?v=<?= define::VERSION; ?>" />
        <link rel="stylesheet" type="text/css" href="/css/style.css?v=<?= define::VERSION; ?>" />

        <!-- FAVICONS -->
        <link rel="shortcut icon" href="/img/favicon/favicon.ico" type="image/x-icon">
        <link rel="icon" href="/img/favicon/favicon.ico" type="image/x-icon">

        <!-- PACE LOADER - turn this on if you want ajax loading to show (caution: uses lots of memory on iDevices)-->
<!--        <script data-pace-options='{ "restartOnRequestAfter": true }' src="/js/plugin/pace/pace.min.js"></script>-->
        
        <!-- JQUERY -->
        <script type="text/javascript" src="/js/jquery-2.1.1.js?v=<?= define::VERSION; ?>"></script>
        <script type="text/javascript" src="/js/jquery-ui-1.10.3.js?v=<?= define::VERSION; ?>"></script>
        
        <!-- BOOTSTRAP JS -->
        <script type="text/javascript" src="/js/bootstrap-3.3.1.js?v=<?= define::VERSION; ?>"></script>
        
        <!-- CUSTOM NOTIFICATION -->
        <script type="text/javascript" src="/js/notification/SmartNotification.min.js"></script>

        <!-- JARVIS WIDGETS -->
        <script type="text/javascript" src="/js/smartwidgets/jarvis.widget.min.js"></script>

        <!-- EASY PIE CHARTS -->
        <script type="text/javascript" src="/js/plugin/easy-pie-chart/jquery.easy-pie-chart.min.js"></script>

        <!-- SPARKLINES -->
        <script type="text/javascript" src="/js/plugin/sparkline/jquery.sparkline.min.js"></script>

        <!-- JQUERY VALIDATE -->
        <script type="text/javascript" src="/js/plugin/jquery-validate/jquery.validate.min.js"></script>

        <!-- JQUERY MASKED INPUT -->
        <script type="text/javascript" src="/js/plugin/masked-input/jquery.maskedinput.min.js"></script>

        <!-- JQUERY SELECT2 INPUT -->
        <script type="text/javascript" src="/js/plugin/select2/select2.min.js"></script>

        <!-- JQUERY UI + Bootstrap Slider -->
        <script type="text/javascript" src="/js/plugin/bootstrap-slider/bootstrap-slider.min.js"></script>

        <!-- browser msie issue fix -->
        <script type="text/javascript" src="/js/plugin/msie-fix/jquery.mb.browser.min.js"></script>
        
        <!-- jsTree -->
        <script type="text/javascript" src="/js/jquery.jstree-3.0.6.js"></script>
        
        <!-- jQuery-contextMenu -->
        <script type="text/javascript" src="/js/jquery.contextMenu-1.6.6.js?v=<?= define::VERSION; ?>"></script>
        
        <!-- jQuery-Guillotine -->
        <script type="text/javascript" src="/js/jquery.guillotine-1.3.1.js?v=<?= define::VERSION; ?>"></script>
        
        <!-- PAGE RELATED PLUGIN(S) -->
        <script type="text/javascript" src="/js/jquery.dataTables-1.10.4.js?v=<?= define::VERSION; ?>"></script>
        <script type="text/javascript" src="/js/jquery.dataTables.ColReorder-1.10.4.js"></script>
        <script type="text/javascript" src="/js/jquery.dataTables.colVis-1.10.4.js"></script>
        <script type="text/javascript" src="/js/jquery.dataTables.fixedColumns-1.10.4.js"></script>
        <script type="text/javascript" src="/js/jquery.dataTables.tableTools-1.10.4.js"></script>
        <script type="text/javascript" src="/js/datatables.responsive.min.js"></script>
        <script type="text/javascript" src="/js/plugin/clockpicker/clockpicker.min.js"></script>
        
        <!-- XI -->
        <script type="text/javascript" src="/js/jquery.dataTables.dataTableExt.oStdClasses.js"></script>
        <script type="text/javascript" src="/js/jquery.dataTables.sort.js?v=<?= define::VERSION; ?>"></script>
        <script type="text/javascript" src="/js/tinymce/tinymce.min.js?v=<?= define::VERSION; ?>"></script>
        <script type="text/javascript" src="/js/my.js?v=<?= define::VERSION; ?>"></script>
        <script type="text/javascript" src="/js/my.strip.js?v=<?= define::VERSION; ?>"></script>
        <script type="text/javascript" src="/js/my.is.js?v=<?= define::VERSION; ?>"></script>
        <script type="text/javascript" src="/js/my.html.js?v=<?= define::VERSION; ?>"></script>
        <script type="text/javascript" src="/js/my.ajax.js?v=<?= define::VERSION; ?>"></script>
        <script type="text/javascript" src="/js/my.file.js?v=<?= define::VERSION; ?>"></script>
    </head>
<body onload="my.html.windowInit(document);">
<?php
    $_debug_ = null;
    if (dlog::is_enabled()) {
        $_debug_ = widget::basic("Debug Information", controller::render($this->template_dir() . "debug.inc"), array("fullwidth" => true));
        $_debug_ .= $this->get_action_output();
    }
?>
<?php if (!empty($header)) : ?>
    <!-- header -->
        <?= $header; ?>
    <!-- end header -->
<?php endif; ?>

<?php if (!empty($sitenav)) : ?>
    <!-- sitenav -->
    <!-- Note: This width of the aside area can be adjusted through LESS variables -->
    <aside id="left-panel">
        <?php if (!empty($logininfo)) : ?>
            <!-- login-info -->
            <div class="login-info">
                <?= $logininfo; ?>
            </div>
            <!-- end login-info -->
        <?php endif; ?>
        <nav>
            <?= $sitenav; ?>
            <span class="minifyme"> <i class="fa fa-arrow-circle-left hit"></i> </span>
        </nav>
    </aside>
    <!-- end sitenav -->
<?php endif; ?>

<?php if (!empty($content)) : ?>
    <!-- main content -->
    <div>
        <?= $content; ?>
        <?php 
            if (dlog::is_enabled()) {
                echo controller::render($this->template_dir() . "debug.inc");
            }
        ?>
    </div>
    <!-- end main content -->
<?php elseif (!empty($window)) : ?>
    <!-- main panel -->
    <div id="main" role="main">
        <!-- main content -->
        <div id="content">
            <?= $window; ?>
            <?php 
                if (dlog::is_enabled()) {
                    echo controller::render($this->template_dir() . "debug.inc");
                }
            ?>
        </div>
        <!-- end main content -->
    </div>
    <!-- end main panel -->
<?php else: ?>
    <!-- main panel -->
    <div id="main" role="main">
        <!-- ribbon -->
        <div id="ribbon">
            <span class="ribbon-button-alignment"> <span id="refresh" class="btn btn-ribbon" data-title="refresh"  rel="tooltip" data-placement="bottom" data-original-title="<i class='text-warning fa fa-warning'></i> Warning! This will reset all your widget settings." data-html="true"><i class="fa fa-refresh"></i></span> </span>
            <?php if (!empty($breadcrumb)) : ?>
                <!-- breadcrumb -->
                <?php 
                    if (is_array($breadcrumb)) {
                        echo template::unordered_list($breadcrumb, array("class" => "breadcrumb"));
                    } else {
                        echo $breadcrumb;
                    }
                ?>
                <!-- end breadcrumb -->
            <?php endif; ?>
            <?php if (!empty($ribbon)) : ?>
                <!-- ribbon -->
                    <?= $ribbon ?>
                <!-- end ribbon -->
            <?php endif; ?>
            <?php if (!empty($ribbonright)) : ?>
                <!-- ribbon ribbonright -->
                <span class="ribbon-button-alignment pull-right">
                    <?= $ribbonright ?>
                </span>
                <!-- end ribbon ribbonright -->
            <?php endif; ?>
        </div>
        <!-- end ribbon -->
        <?php if (!empty($widget) || $_debug_) : ?>
            <!-- widget grid -->
            <div id="content">
                <section id="widget-grid" class="">
                    <div class="row">
                        <?= $widget; ?>
                        <?= $_debug_; ?>
                    </div>
                </section>
            </div>
            <!-- end widget grid -->
        <?php endif; ?>
    </div>
    <!-- end main panel -->
<?php endif; ?>

<!-- smartadmin MAIN APP JS FILE -->
<script type="text/javascript" src="/js/app.js"></script>
</body>
</html>
