<div class="smart-form"><!-- smart form !-->
<input type="hidden" name="action" value="controller_user.save" />
<input type="hidden" name="<?= session::requestkey(); ?>" value="<?= session::requestid(); ?>" />
<input type="hidden" name="user[userid]" value="<?= $model['user']['userid']['value'] ?>"/>
<fieldset>
        <div class="row">
            <section class="col col-3">
                <label class="label">Title</label>
                <label class="select<?php if ($model['user']['forename']['error']) echo " has-error"; ?>" for="usertitle">
                    <select name="user[title]" id="usertitle">
                        <?php
                            $options = array(
                                "" => "Title",
                                "Mr" => "Mr",
                                "Mrs" => "Mrs",
                                "Miss" => "Miss",
                                "Ms" => "Ms",
                                "Dr" => "Dr",
                                "Rev" => "Rev"
                            );
                            
                            $name = "title";
                            $disabled = " disabled=\"disabled\"";
                            $optionlist = "";
                            foreach ($options as $key => $value) {
                                $selected = "";
                                if ($key == $model['user'][$name]['value']) {
                                    $selected = " selected=\"selected\"";
                                }
                                $optionlist .= "<option value=\"{$key}\"{$selected}{$disabled}>{$value}</option>\n";
                                $disabled = "";
                            }
                            echo $optionlist;
                        ?>
                    </select> <i></i> </label>
            </section>
        </div>
        <div class="row">
            <section class="col col-6">
                <label class="label" for="userforename">First name</label>
                <label class="input<?php if ($model['user']['forename']['error']) echo " has-error"; ?>"> <i class="icon-prepend fa fa-user"></i>
                    <input type="text" name="user[forename]" id="userforename" value="<?= $model['user']['forename']['value']; ?>" placeholder="First name">
                    <?= template::error_message($model['user']['forename']['error']); ?>
                </label>
            </section>
            <section class="col col-6">
                <label class="label" for="usersurname">Last name</label>
                <label class="input<?php if ($model['user']['surname']['error']) echo " has-error"; ?>"> <i class="icon-prepend fa fa-user"></i>
                    <input type="text" name="user[surname]" id="usersurname" value="<?= $model['user']['surname']['value']; ?>" placeholder="Last name">
                    <?= template::error_message($model['user']['surname']['error']); ?>
                </label>
            </section>
        </div>
        <div class="row">
            <section class="col col-6">
                <label class="label" for="userprevioussurname">Previous last name</label>
                <label class="input<?php if ($model['user']['previoussurname']['error']) echo " has-error"; ?>"> <i class="icon-prepend fa fa-user"></i>
                    <input type="text" name="user[previoussurname]" id="userprevioussurname" value="<?= $model['user']['previoussurname']['value']; ?>" placeholder="Previous last name">
                    <?= template::error_message($model['user']['previoussurname']['error']); ?>
                </label>
            </section>
            <section class="col col-6">
                <label class="label" for="userdateofbirth">Date of birth</label>
                <label class="input<?php if ($model['user']['dateofbirth']['error']) echo " has-error"; ?>"> <i class="icon-prepend fa fa-user"></i>
                    <input class="dateofbirth" type="text" name="user[dateofbirth]" id="userdateofbirth" value="<?= $model['user']['dateofbirth']['value']; ?>" placeholder="Date of birth">
                    <?= template::error_message($model['user']['dateofbirth']['error']); ?>
                </label>
            </section>
        </div>
        <div class="row">
            <section class="col col-6">
                <label class="label" for="userhomephone">Home phone</label>
                <label class="input<?php if ($model['user']['phone']['value'][0]['phone']['phone']['error']) echo " has-error"; ?>"> <i class="icon-prepend fa fa-phone"></i>
                    <input type="tel" name="user[phone][0][phone]" id="userhomephone" value="<?= $model['user']['phone']['value'][0]['phone']['phone']['value']; ?>" data-mask="(999) 999-9999" placeholder="Home phone">
                    <?= template::error_message($model['user']['phone']['value'][0]['phone']['phone']['error']); ?>
                </label>
                <input type="hidden" name="user[phone][0][phoneid]" value="<?= $model['user']['phone']['value'][0]['phone']['phoneid']['value']; ?>">
            </section>
            <section class="col col-6">
                <label class="label" for="usermobilephone">Mobile phone</label>
                <label class="input<?php if ($model['user']['phone']['value'][1]['phone']['phone']['error']) echo " has-error"; ?>"> <i class="icon-prepend fa fa-phone"></i>
                    <input type="tel" name="user[phone][1][phone]" id="usermobilephone" value="<?= $model['user']['phone']['value'][1]['phone']['phone']['value']; ?>" data-mask="(999) 999-9999" placeholder="Mobile phone">
                    <?= template::error_message($model['user']['phone']['value'][1]['phone']['phone']['error']); ?>
                </label>
                <input type="hidden" name="user[phone][1][phoneid]" value="<?= $model['user']['phone']['value'][1]['phone']['phoneid']['value']; ?>">
            </section>
        </div>
        <div class="row">
            <section class="col col-6">
                <label class="label" for="useremail">Email</label>
                <label class="input<?php if ($model['user']['email']['error']) echo " has-error"; ?>"> <i class="icon-prepend fa fa-envelope-o"></i>
                    <input type="email" name="user[email]" id="useremail" value="<?= $model['user']['email']['value']; ?>" placeholder="Email">
                    <?= template::error_message($model['user']['email']['error']); ?>
                </label>
            </section>
        </div>
</fieldset>
<footer>
    <button type="submit" class="btn btn-primary">
        Save
    </button>
</footer>
</div><!-- smart form !-->