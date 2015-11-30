/*

ability Foo
  def bar x, y
    var z = 10 + x - y
    var l = yield z, 2, 4
    var m = 20;
  end
  def baz
    var r = 10
    var s = 20
    bar(r, s) do |v, w, z|
      print v
      if v == 20
          return(5);
      end
      7
    end
    var t = 100;
  end

  def blot
    var w = 10
    bar(w, nil) do |v|
      var m = 7
      yield 6,m,v
    end
    var q = 11
  end

  def bee
    var 
    blot do
      var f = 9
      yield f
    end
  end
  
end



*/

thing *bar(thing x, thing y, jmp_buf block_env, jmp_buf caller_env, thing * p1, thing * p2, thing * p3) {
    thing _acme_;
    thing _acme_z;
    _acme_ = _acme_z = call_send(1, call_send(1, new_i_thing(10), new_sym_thing("+"), x, NULL), new_sym_thing("-"), y, NULL);
    thing _acme_l;
    {
        /* This is the yield statement itself */
        block_ret br = block_caller_yield_outer(caller_env);
        thing ret = br.t;
        acme_int ret_type = br.ret_type;
        /* This sets caller env */
        if(!ret_type) {
            (*p1) = _acme_z;
            (*p2) = new_i_thing(2);
            (*p3) = new_i_thing(4);
            block_caller_yield_inner(block_env);
            }
        }
        else if(ret_type!=ACME_BLOCK_NEXT) {
            block_caller_leave(block_env, ret_type)
        }
        else { /* BLOCK_NEXT */
            _acme_l = ret;
        }
    }
    thing _acme_m;
    _acme_ = _acme_m = new_i_thing(20);
    return _acme_;
}

thing *baz(void) {
    thing _acme_;
    thing _acme_r;
    _acme_ = _acme_r = new_i_thing(10);
    thing _acme_s;
    _acme_ = _acme_s = new_i_thing(20);
    {
        thing _acme_v;
        thing _acme_w;
        thing _acme_z;
        jmp_buf caller_env;
        acme_int ret_type;
        if(!(ret_type = block_define(block_env))) {
            _acme_ = call_send(0, get_self(), new_sym_thing("bar"), r, s, block_env, caller_env, &v, &w, &z);
        }
        else if(ret_type == ACME_BLOCK_INVOKE) {
            /* Caller env will be magically set by the time we get here */
            _acme_ = print(_acme_v);
            _acme_ - call_send(1, _acme_v, new_sym_thing("=="), ...);
            if(_acme_.u.i == 1) {
                _acme_ = new_i_thing(5)
                block_leave(caller_env, _acme_, ACME_BLOCK_RETURN);
            }
            _acme_ = new_i_thing(7);
            block_leave(caller_env, _acme_, ACME_BLOCK_NEXT);
        }
        else if(ret_type == ACME_BLOCK_RETURN) {
            return _acme_;
        }
        /* if i == ACME_BLOCK_BREAK, just fall through */
    }
    thing _acme_t;
    _acme_t = new_i_thing(100);
    return _acme_;
}
