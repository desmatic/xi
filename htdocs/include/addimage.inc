<div class="modal-header">
    <button type="button" class="close" data-dismiss="modal" aria-hidden="true">
            &times;
    </button>
    <h4 class="modal-title">Edit Image</h4> 
</div>
<div class="modal-body">
    <div class="row">
        <div class="col-md-12">
            <img id="guillotine" src="/images/croptest.jpeg" alt="">
        </div>
    </div>
    <div class="row">
        <div class="col-md-12">
            <h1 id="controls" class="textcenter">
                <a href='#' id='rotateLeft'  title='Rotate left'><i class='fa fa-rotate-left'></i></a>&nbsp;&nbsp;
                <a href='#' id='rotateRight' title='Rotate right'><i class='fa fa-rotate-right'></i></a>&nbsp;&nbsp;
                <a href='#' id='fit'         title='Fit image'><i class='fa fa-arrows-alt'></i></a>&nbsp;&nbsp;
                <a href='#' id='zoomOut'     title='Zoom out'><i class='fa fa-search-minus'></i></a>&nbsp;&nbsp;
                <a href='#' id='zoomIn'      title='Zoom in'><i class='fa fa-search-plus'></i></a> 
            </h1>
        </div>
    </div>
</div>
<div class="modal-footer">
    <button type="button" class="btn btn-default" data-dismiss="modal">
            Cancel
    </button>
    <button type="button" class="btn btn-primary">
            Save Image
    </button>
</div>
<script type="text/javascript">
jQuery(function() {
    var picture = jQuery('#guillotine');
    picture.on('load', function() {
        picture.guillotine({ width: 896, height: 504 });
        picture.guillotine('center');
        jQuery('#controls a').click(function(e) {
            e.preventDefault()
            picture.guillotine(this.id)
        })
    });
});
</script>