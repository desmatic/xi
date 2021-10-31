                                <span> <!-- User image size is adjusted inside CSS, it should stay as it --> 
                                        
                                        <a href="javascript:void(0);" id="show-shortcut">
                                                <img src="/thumb.html?md5sum=<?= session::user()->md5sum; ?>&amp;bytes=<?= session::user()->bytes; ?>&amp;size=thumb128x128" alt="me" class="online" /> 
                                                <span>
                                                        <?= session::usernickname(); ?>
                                                </span>
                                        </a> 
                                        
                                </span>